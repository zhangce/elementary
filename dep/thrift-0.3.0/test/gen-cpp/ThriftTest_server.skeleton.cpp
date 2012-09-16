// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ThriftTest.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace thrift::test;

class ThriftTestHandler : virtual public ThriftTestIf {
 public:
  ThriftTestHandler() {
    // Your initialization goes here
  }

  void testVoid() {
    // Your implementation goes here
    printf("testVoid\n");
  }

  void testString(std::string& _return, const std::string& thing) {
    // Your implementation goes here
    printf("testString\n");
  }

  int8_t testByte(const int8_t thing) {
    // Your implementation goes here
    printf("testByte\n");
  }

  int32_t testI32(const int32_t thing) {
    // Your implementation goes here
    printf("testI32\n");
  }

  int64_t testI64(const int64_t thing) {
    // Your implementation goes here
    printf("testI64\n");
  }

  double testDouble(const double thing) {
    // Your implementation goes here
    printf("testDouble\n");
  }

  void testStruct(Xtruct& _return, const Xtruct& thing) {
    // Your implementation goes here
    printf("testStruct\n");
  }

  void testNest(Xtruct2& _return, const Xtruct2& thing) {
    // Your implementation goes here
    printf("testNest\n");
  }

  void testMap(std::map<int32_t, int32_t> & _return, const std::map<int32_t, int32_t> & thing) {
    // Your implementation goes here
    printf("testMap\n");
  }

  void testSet(std::set<int32_t> & _return, const std::set<int32_t> & thing) {
    // Your implementation goes here
    printf("testSet\n");
  }

  void testList(std::vector<int32_t> & _return, const std::vector<int32_t> & thing) {
    // Your implementation goes here
    printf("testList\n");
  }

  Numberz testEnum(const Numberz thing) {
    // Your implementation goes here
    printf("testEnum\n");
  }

  UserId testTypedef(const UserId thing) {
    // Your implementation goes here
    printf("testTypedef\n");
  }

  void testMapMap(std::map<int32_t, std::map<int32_t, int32_t> > & _return, const int32_t hello) {
    // Your implementation goes here
    printf("testMapMap\n");
  }

  void testInsanity(std::map<UserId, std::map<Numberz, Insanity> > & _return, const Insanity& argument) {
    // Your implementation goes here
    printf("testInsanity\n");
  }

  void testMulti(Xtruct& _return, const int8_t arg0, const int32_t arg1, const int64_t arg2, const std::map<int16_t, std::string> & arg3, const Numberz arg4, const UserId arg5) {
    // Your implementation goes here
    printf("testMulti\n");
  }

  void testException(const std::string& arg) {
    // Your implementation goes here
    printf("testException\n");
  }

  void testMultiException(Xtruct& _return, const std::string& arg0, const std::string& arg1) {
    // Your implementation goes here
    printf("testMultiException\n");
  }

  void testOneway(const int32_t secondsToSleep) {
    // Your implementation goes here
    printf("testOneway\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<ThriftTestHandler> handler(new ThriftTestHandler());
  shared_ptr<TProcessor> processor(new ThriftTestProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

