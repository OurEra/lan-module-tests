# build library
make linux-debug EXTRA_CFLAGS="-DMD_HAVE_EPOLL"

# build example
gcc examples/server.c examples/error.c -ILINUX_5.4.0-65-generic_DBG LINUX_5.4.0-65-generic_DBG/libst.a -o test-server
gcc ../state_thread_example_client.c -o test-client

# run server
./test-server -l dbghost -b 100.100.35.84:8556 -i -t 2
