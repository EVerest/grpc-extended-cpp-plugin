#ifndef GRPC_EXTENDED_CPP_PLUGIN_H
#define GRPC_EXTENDED_CPP_PLUGIN_H

#include "src/compiler/cpp_plugin.h"

#include "extended_cpp_generator.h"

class ExtendedCppGrpcGenerator : public CppGrpcGenerator {
    public:
        ExtendedCppGrpcGenerator() {}
        virtual ~ExtendedCppGrpcGenerator() {}
        virtual bool Generate(const grpc::protobuf::FileDescriptor* file,
                              const std::string& parameter,
                              grpc::protobuf::compiler::GeneratorContext* generator_context,
                              std::string* error) const override {
            bool result = CppGrpcGenerator::Generate(file, parameter, generator_context, error);
            if (!result) {
                return false;
            }
            std::string file_name = grpc_generator::StripProto(file->name());

            // extend existing generated files
            this->Insert(
                generator_context,
                file_name + ".pb.h",
                "namespace_scope",
                extended_cpp_generator::GetHeaderCreateFunctions(file)
            );
            this->Insert(
                generator_context,
                file_name + ".pb.cc",
                "namespace_scope",
                extended_cpp_generator::GetSourceCreateFunctions(file)
            );

            // create new files
            std::string header_code = 
                extended_cpp_generator::GetHeaderPrologue(file) +
                extended_cpp_generator::GetHeaderIncludes(file) +
                extended_cpp_generator::GetHeaderServices(file) +
                extended_cpp_generator::GetHeaderEpilogue(file);
            std::unique_ptr<grpc::protobuf::io::ZeroCopyOutputStream> header_output(
                generator_context->Open(file_name + ".grpc-ext.pb.h")
            );
            grpc::protobuf::io::CodedOutputStream header_coded_out(header_output.get());
            header_coded_out.WriteRaw(header_code.data(), header_code.size());

            std::string source_code = 
                extended_cpp_generator::GetSourcePrologue(file) +
                extended_cpp_generator::GetSourceIncludes(file) +
                extended_cpp_generator::GetSourceServices(file) +
                extended_cpp_generator::GetSourceEpilogue(file);
            std::unique_ptr<grpc::protobuf::io::ZeroCopyOutputStream> source_output(
                generator_context->Open(file_name + ".grpc-ext.pb.cc")
            );
            grpc::protobuf::io::CodedOutputStream source_coded_out(source_output.get());
            source_coded_out.WriteRaw(source_code.data(), source_code.size());

            return true;
        }
    private:
          // Insert the given code into the given file at the given insertion point.
        void Insert(grpc::protobuf::compiler::GeneratorContext* context,
                    const std::string& filename,
                    const std::string& insertion_point,
                    const std::string& code) const {
            std::unique_ptr<grpc::protobuf::io::ZeroCopyOutputStream> output(
            context->OpenForInsert(filename, insertion_point));
            grpc::protobuf::io::CodedOutputStream coded_out(output.get());
            coded_out.WriteRaw(code.data(), code.size());
        }
};


#endif  // GRPC_EXTENDED_CPP_PLUGIN_H
