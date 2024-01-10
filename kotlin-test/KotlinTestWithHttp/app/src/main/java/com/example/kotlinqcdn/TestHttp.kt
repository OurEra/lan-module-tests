package com.example.kotlinqcdn

import android.text.TextUtils
import android.util.Log
import okhttp3.Headers
import okhttp3.Headers.Companion.toHeaders
import okhttp3.HttpUrl.Companion.toHttpUrlOrNull
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.Response
import java.io.BufferedReader
import java.io.IOException
import java.io.InputStream
import java.io.InputStreamReader
import java.io.OutputStream
import java.net.InetAddress
import java.net.InetSocketAddress
import java.net.ServerSocket
import java.net.Socket
import java.util.Locale
import java.util.concurrent.ConcurrentHashMap
import java.util.concurrent.CountDownLatch
import java.util.concurrent.LinkedBlockingDeque
import java.util.concurrent.ThreadFactory
import java.util.concurrent.ThreadPoolExecutor
import java.util.concurrent.TimeUnit
import javax.net.ServerSocketFactory

class TestHttp {

    data class Config(var timeout: Int)
    data class Strategy(val backup: String, val boot: String)
    private data class UrlBase(var scheme: String, var host: String)
    private data class UrlStrategy(var backup: UrlBase, var boot: UrlBase)
    private data class Resource(var base: UrlBase, var rawPath: String)
    private data class RequestInfo(
        var method: String,
        var uri: String,
        var host: String?,
        var headers: Headers?
    )

    private val MAX_PARALLEL_THREAD_NUM = 8
    private val LOCAL_SERVER_IP = "127.0.0.1"
    private val HEADER_CONNECTION = "Connection"

    private val HEADER_HOST = "Host"
    private val HTTP_TOO_MANY_REQUEST = 429
    private val EMPTY_LINE = "\n".encodeToByteArray()
    private val STATUS_LINE_500 = "HTTP/1.1 500 Server Error\n".encodeToByteArray()

    private var localServerStart = false
    private var address: String = ""

    private val client: OkHttpClient
    private lateinit var executor: ThreadPoolExecutor
    private lateinit var serverSocket: ServerSocket

    private val strategies: MutableMap<UrlBase, UrlStrategy> = ConcurrentHashMap()
    private val redirects: MutableMap<Resource, Resource> = ConcurrentHashMap()

    constructor(config: Config?) {
        var useConfig = config
        if (useConfig == null) {
            useConfig = Config(1000)
        }
        client =
            OkHttpClient.Builder().connectTimeout(useConfig.timeout.toLong(), TimeUnit.MILLISECONDS)
                .build()
        startLocalServer()
    }

    fun setStrategy(base: String, strategy: Strategy) {
        val baseUrl = urlBaseOf(base)
        val backup = urlBaseOf(strategy.backup)
        val boot = urlBaseOf(strategy.boot)
        strategies.put(baseUrl, UrlStrategy(backup, boot))
    }

    fun makeVodUrl(url: String): String {
        if (!localServerStart) {
            return url
        }
        try {
            var httpUrl = HttpUrl(url)
            val strategy = strategies.get(UrlBase(httpUrl.scheme, httpUrl.host))
            if (strategy == null) {
                Log.i("Test", "no strategy")
                return url
            }

            httpUrl.rawPath = makeProxyPath(httpUrl.scheme, httpUrl.host, httpUrl.rawPath)
            httpUrl.scheme = "http"
            httpUrl.host = getProxyHost()
            return httpUrl.toString()
        } catch (e: Exception) {
            return url
        }
    }

    private fun makeProxyPath(scheme: String, host: String, rawPath: String): String {
        return "/" + scheme + "," + host + rawPath
    }

    private fun getProxyHost(): String {
        return LOCAL_SERVER_IP + ":" + serverSocket.localPort
    }

    private fun startLocalServer() {
        executor = ThreadPoolExecutor(MAX_PARALLEL_THREAD_NUM, Int.MAX_VALUE, 2, TimeUnit.SECONDS,
            LinkedBlockingDeque(), object : ThreadFactory {
                var index: Int = 0
                override fun newThread(p0: Runnable?): Thread {
                    index++
                    val thread = Thread(p0)
                    thread.name = "Test-thread-" + index
                    return thread
                }
            })

        try {
            val addr = InetSocketAddress(InetAddress.getByName(LOCAL_SERVER_IP), 0)
            serverSocket = ServerSocketFactory.getDefault().createServerSocket()
            serverSocket.bind(addr)

            val countdown = CountDownLatch(1)
            Thread(
                object : Runnable {
                    override fun run() {
                        countdown.countDown()
                        acceptConnection()
                    }
                }
            ).start()
            countdown.await()
            address = "http://" + serverSocket.localSocketAddress.toString()
            localServerStart = true
            Log.i("Test", "start server: " + address)
        } catch (e: Exception) {
            Log.i("Test", "e " + e.toString())
        }
    }

    private fun acceptConnection() {
        while (true) {
            var socket: Socket
            try {
                socket = serverSocket.accept()
            } catch (e: IOException) {
                Log.i("Test", "accept err " + e.toString())
                return
            }
            executor.execute {
                processSocket(socket)
            }
        }
    }

    private fun processSocket(socket: Socket) {
        val requestInfo = readRequest(socket.getInputStream())
        val httpUrl = HttpUrl(address + requestInfo.uri)

        val uri = parseProxyPath(httpUrl.rawPath)
        if (uri == null) {
            writeErrorResponse(socket.getOutputStream(), "invalid proxy path")
            return
        }

        val strategy = strategies.get(uri.base)
        if (strategy == null) {
            writeErrorResponse(socket.getOutputStream(), "strategy not found")
            return
        }

        val urlRedirect = redirectOf(uri)
        if (urlRedirect != null) {
            httpUrl.rawPath = urlRedirect.rawPath;
            httpUrl.scheme = urlRedirect.base.scheme
            httpUrl.host = urlRedirect.base.host
            val headers = requestInfo.headers?.newBuilder()?.set(HEADER_HOST, httpUrl.host)?.build()

            val request =
                headers?.let { Request.Builder().url(httpUrl.toString()).headers(it).build() }
            Log.i("Test", "redirect serveRequest " + request)
            val response = request?.let { serveRequest(socket.getOutputStream(), it, uri) }
            if (response != null) {
                Log.e("Test", "redirect ok " + request)
                return
            }
            Log.e("Test", "redirect fail")
        } else {
            httpUrl.rawPath = uri.rawPath;
            httpUrl.scheme = uri.base.scheme
            httpUrl.host = uri.base.host
            val headers = requestInfo.headers?.newBuilder()?.set(HEADER_HOST, httpUrl.host)?.build()

            val request =
                headers?.let { Request.Builder().url(httpUrl.toString()).headers(it).build() }
            Log.i("Test", "direct serveRequest " + request)
            val response = request?.let { serveRequest(socket.getOutputStream(), it, uri) }
            if (response != null) {
                Log.e("Test", "direct ok " + request)
                return
            }
            Log.e("Test", "direct fail")
        }

        writeErrorResponse(socket.getOutputStream(), "backup/redirect fail")

        socket.close()
    }

    private fun readRequest(inputStream: InputStream): RequestInfo {
        val reader = BufferedReader(InputStreamReader(inputStream))
        val parts = reader.readLine().split(" ", limit = 3)
        val requestInfo = RequestInfo(parts[0], parts[1], null, null)

        val headers = HashMap<String, String>()
        var line: String? = reader.readLine()
        while (line != null && line.length > 0) {
            val keyvalue = line.split(": ", limit = 2)
            headers.put(keyvalue[0], keyvalue[1])
            if (keyvalue[0].equals(HEADER_HOST)) {
                requestInfo.host = keyvalue[1]
            }
            line = reader.readLine()
        }
        requestInfo.headers = headers.toHeaders()
        return requestInfo
    }

    private fun serveRequest(output: OutputStream, request: Request, uri: Resource): Response? {
        val response = client.newCall(request).execute()
        try {
            if (response.code != HTTP_TOO_MANY_REQUEST && response.code / 100 != 5) {
                if (response.priorResponse != null) {
                    val requestUrl = HttpUrl(response.request.url)
                    val base = UrlBase(requestUrl.scheme, requestUrl.host)
                    setRedirect(uri, Resource(base, requestUrl.rawPath))
                }
                forwardResponse(output, response)
                return response
            }
        } catch (e: java.lang.Exception) {
            Log.e("Test", "errpr: " + e.toString())
        }
        return null
    }

    private fun urlBaseOf(url: String): UrlBase {
        val httpUrl = HttpUrl(url)
        return UrlBase(httpUrl.scheme, httpUrl.host)
    }

    private fun setRedirect(uri: Resource, to: Resource?) {
        if (to == null) {
            redirects.remove(uri)
        } else {
            redirects.put(uri, to)
        }
    }

    private fun redirectOf(uri: Resource): Resource? {
        return redirects.get(uri)
    }

    private fun writeErrorResponse(output: OutputStream, err: String) {
        val body = err.encodeToByteArray()
        output.write(STATUS_LINE_500)
        writeResponseHeader(output, "Content-Length", body.size.toString())
        writeResponseHeader(output, HEADER_CONNECTION, "close")
        output.write(EMPTY_LINE)
        output.write(body)
    }

    private fun writeResponseHeader(output: OutputStream, key: String, value: String) {
        output.write((key + ": " + value + "\n").encodeToByteArray())
    }

    private fun parseProxyPath(path: String): Resource? {
        val parts = path.substring(1).split("/", limit = 2)
        if (parts.size != 2) {
            return null
        }
        val schemeAndHost = parts[0].split(",", limit = 2)
        if (schemeAndHost.size != 2) {
            return null
        }
        val path = "/" + parts[1]
        return Resource(UrlBase(schemeAndHost[0], schemeAndHost[1]), path)
    }

    private fun forwardResponse(output: OutputStream, response: Response) {
        // status line
        val status = response.protocol.toString()
            .uppercase(Locale.ROOT) + " " + response.code + " " + response.message + "\n"
        output.write(status.encodeToByteArray())

        // headers
        val headers = response.headers.newBuilder().set(HEADER_CONNECTION, "close").build()
        output.write(headers.toString().encodeToByteArray())
        output.write(EMPTY_LINE)

        // body
        val input = response.body?.byteStream()
        var readSize = -1
        var readBuffer = ByteArray(4 * 1024)
        if (input != null) {
            readSize = input.read(readBuffer)
        }
        while (readSize != -1) {
            output.write(readBuffer, 0, readSize)
            if (input != null) {
                readSize = input.read(readBuffer)
            }
        }
        input?.close()
    }

    private class HttpUrl {
        lateinit var scheme: String
        lateinit var host: String
        lateinit var rawPath: String
        var rawQuery: String? = null

        constructor(url: String) : this(url.toHttpUrlOrNull()) {
        }

        constructor(url: okhttp3.HttpUrl?) {
            if (url != null) {
                this.scheme = url.scheme
                if (url.port != okhttp3.HttpUrl.defaultPort(this.scheme)) {
                    this.host = url.host + ":" + url.port
                } else {
                    this.host = url.host
                }
                this.rawPath = url.encodedPath
                this.rawQuery = url.encodedQuery
            }
        }

        override fun toString(): String {
            if (TextUtils.isEmpty(rawQuery)) {
                return scheme + "://" + host + rawPath;
            } else {
                return scheme + "://" + host + rawPath + "?" + rawQuery;
            }
        }
    }
}
