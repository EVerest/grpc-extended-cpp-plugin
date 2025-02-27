#ifndef EXTENDED_CPP_GENERATOR_H
#define EXTENDED_CPP_GENERATOR_H

#include <string>

#include "src/compiler/config.h"

namespace extended_cpp_generator {

std::string GetHeaderCreateFunctions(const grpc::protobuf::FileDescriptor* file);
std::string GetSourceCreateFunctions(const grpc::protobuf::FileDescriptor* file);

std::string GetHeaderPrologue(const grpc::protobuf::FileDescriptor* file);
std::string GetHeaderIncludes(const grpc::protobuf::FileDescriptor* file);
std::string GetHeaderServices(const grpc::protobuf::FileDescriptor* file);
std::string GetHeaderEpilogue(const grpc::protobuf::FileDescriptor* file);

std::string GetSourcePrologue(const grpc::protobuf::FileDescriptor* file);
std::string GetSourceIncludes(const grpc::protobuf::FileDescriptor* file);
std::string GetSourceServices(const grpc::protobuf::FileDescriptor* file);
std::string GetSourceEpilogue(const grpc::protobuf::FileDescriptor* file);

} // namespace extended_cpp_generator


#endif // EXTENDED_CPP_GENERATOR_H
