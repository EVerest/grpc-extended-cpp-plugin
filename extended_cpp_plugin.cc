#include "extended_cpp_plugin.h"

#include "src/compiler/protobuf_plugin.h"

int main(int argc, char* argv[]) {
    ExtendedCppGrpcGenerator generator;
    return grpc::protobuf::compiler::PluginMain(argc, argv, &generator);
}
