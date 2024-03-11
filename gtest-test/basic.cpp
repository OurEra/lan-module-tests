#include <math.h>
#include <stdint.h>
#include "curl/curl.h"
#include "gtest/gtest.h"

static size_t write_cb(void *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  return realsize;
}

static size_t header_cb(char *buffer, size_t size,
                              size_t nitems, void *userdata)
{
  return nitems * size;
}

TEST(HelloGtest, GtestCase0) {
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);

  EXPECT_EQ(curl_easy_perform(curl), CURLE_OK);
  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  EXPECT_EQ(response_code, 200);
}

class HelloGtestSuit : public ::testing::Test {
public:
  void SetUp() override {
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);
  }

  void TearDown() override {
  }
  CURL *curl;
};

TEST_F(HelloGtestSuit, GtestCase1) {
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
  EXPECT_EQ(curl_easy_perform(curl), CURLE_OK);
  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  EXPECT_EQ(response_code, 200);
}

class TestParameter {
public:
  TestParameter(std::string url, std::string range):
          url_(url), range_(range) {}

  std::string url_;
  std::string range_;
};

class HelloGtestSuitParameter : public ::testing::TestWithParam<TestParameter> {
 public:
  const TestParameter& getParameter() const { return GetParam(); }

};

TEST_P(HelloGtestSuitParameter, GtestCase2) {
  const TestParameter& para = getParameter();
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, para.url_.c_str());
  curl_easy_setopt(curl, CURLOPT_RANGE, para.range_.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);

  EXPECT_EQ(curl_easy_perform(curl), CURLE_OK);
  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  EXPECT_EQ(response_code, 206);

  curl_off_t content_length;
  curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &content_length);
  printf("content length %ld\n", content_length);
}

INSTANTIATE_TEST_SUITE_P(
    PameterCase,
    HelloGtestSuitParameter,
    ::testing::Values(
            TestParameter("http://vjs.zencdn.net/v/oceans.mp4", "0-1023"),
            TestParameter("http://vjs.zencdn.net/v/oceans.mp4", "0-1048575")
            ));

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
