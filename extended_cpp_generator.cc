#include "extended_cpp_generator.h"

#define TAB_SIZE 2

namespace extended_cpp_generator {

std::string CppNameFromProtoName(const std::string& proto_name) {
    std::string cpp_name = proto_name;
    size_t start_pos = 0;
    while ((start_pos = cpp_name.find(".", start_pos)) != std::string::npos) {
        cpp_name.replace(start_pos, 1, "::");
        start_pos += 2;
    }
    return cpp_name;
}

std::string StubTypeFromService(const grpc::protobuf::ServiceDescriptor* service) {
    std::string stub_type;
    stub_type = CppNameFromProtoName(service->full_name()) + "::Stub";
    stub_type = "std::shared_ptr<" + stub_type + ">";
    return stub_type;
}

std::string Tabs(int count) {
    return std::string(count * TAB_SIZE, ' ');
}

std::string ConvertToCppType(google::protobuf::FieldDescriptor::CppType type) {
    std::string cpp_type;
    switch (type) {
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT32:
            cpp_type = "int32_t";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_INT64:
            cpp_type = "int64_t";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
            cpp_type = "uint32_t";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT64:
            cpp_type = "uint64_t";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_DOUBLE:
            cpp_type = "double";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
            cpp_type = "float";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
            cpp_type = "bool";
            break;
        case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING:
            cpp_type = "std::string";
            break;
        default:
            throw std::runtime_error("Unsupported type");
    }
    return cpp_type;
}

std::string GetHeaderCreateFunctions(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;
    for(int message_index = 0; message_index < file->message_type_count(); message_index++) {
        const google::protobuf::Descriptor* message = file->message_type(message_index);
        std::string message_name = message->name();
        output << message_name << " Create" << message_name << "(";

        if (message->field_count() > 0) {
            output << "\n";
        }
        // function paramters
        for (int field_index = 0; field_index < message->field_count(); field_index++) {
            const google::protobuf::FieldDescriptor* field = message->field(field_index);
            std::string field_name = field->name();
            google::protobuf::FieldDescriptor::CppType type = field->cpp_type();
            std::string field_type;
            if (type == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                const google::protobuf::Descriptor* nested_message = field->message_type();
                field_type = CppNameFromProtoName(nested_message->full_name()) + "*";
            } else if (type == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                const google::protobuf::EnumDescriptor* enum_type = field->enum_type();
                field_type = CppNameFromProtoName(enum_type->full_name());
            } else {
                field_type = ConvertToCppType(type);
            }
            if (field->is_map()) {
                auto key_field = field->message_type()->field(0);
                std::string key_type;
                if (key_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                    key_type = CppNameFromProtoName(key_field->message_type()->full_name()) + "*";
                } else if (key_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                    key_type = CppNameFromProtoName(key_field->enum_type()->full_name());
                } else {
                    key_type = ConvertToCppType(key_field->cpp_type());
                }
                auto value_field = field->message_type()->field(1);
                std::string value_type;
                if (value_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                    value_type = CppNameFromProtoName(value_field->message_type()->full_name());
                } else if (value_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                    value_type = CppNameFromProtoName(value_field->enum_type()->full_name());
                } else {
                    value_type = ConvertToCppType(value_field->cpp_type());
                }
                field_type = "std::map<" + key_type + ", " + value_type + ">";
            } else if (field->is_repeated()) {
                field_type = "std::vector<" + field_type + ">";
            }
            output << Tabs(2) << field_type << " " << field_name;
            if (field_index < message->field_count() - 1) {
                output << ",\n";
            }
        }
        output << ");\n\n";
    }
    return output.str();
}

std::string GetSourceCreateFunctions(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;
    for(int message_index = 0; message_index < file->message_type_count(); message_index++) {
        const google::protobuf::Descriptor* message = file->message_type(message_index);
        std::string message_name = message->name();
        output << message_name << " Create" << message_name << "(";

        if (message->field_count() > 0) {
            output << "\n";
        }
        // function paramters
        for (int field_index = 0; field_index < message->field_count(); field_index++) {
            const google::protobuf::FieldDescriptor* field = message->field(field_index);
            std::string field_name = field->name();
            google::protobuf::FieldDescriptor::CppType type = field->cpp_type();
            std::string field_type;
            if (type == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                const google::protobuf::Descriptor* nested_message = field->message_type();
                field_type = CppNameFromProtoName(nested_message->full_name()) + "*";
            } else if (type == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                const google::protobuf::EnumDescriptor* enum_type = field->enum_type();
                field_type = CppNameFromProtoName(enum_type->full_name());
            } else {
                field_type = ConvertToCppType(type);
            }
            if (field->is_map()) {
                auto key_field = field->message_type()->field(0);
                std::string key_type;
                if (key_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                    key_type = CppNameFromProtoName(key_field->message_type()->full_name()) + "*";
                } else if (key_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                    key_type = CppNameFromProtoName(key_field->enum_type()->full_name());
                } else {
                    key_type = ConvertToCppType(key_field->cpp_type());
                }
                auto value_field = field->message_type()->field(1);
                std::string value_type;
                if (value_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                    value_type = CppNameFromProtoName(value_field->message_type()->full_name());
                } else if (value_field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM) {
                    value_type = CppNameFromProtoName(value_field->enum_type()->full_name());
                } else {
                    value_type = ConvertToCppType(value_field->cpp_type());
                }
                field_type = "std::map<" + key_type + ", " + value_type + ">";
            } else if (field->is_repeated()) {
                field_type = "std::vector<" + field_type + ">";
            }
            output << Tabs(2) << field_type << " " << field_name;
            if (field_index < message->field_count() - 1) {
                output << ",\n";
            }
        }
        output << ") {\n";
        // function body
        output << Tabs(1) << message_name << " res;\n";
        for (int field_index = 0; field_index < message->field_count(); field_index++) {
            const google::protobuf::FieldDescriptor* field = message->field(field_index);
            std::string field_name = field->name();
            if (field->is_repeated()) {
                output << Tabs(1) << "res.mutable_" << field_name << "()->";
                if (field->is_map()) {
                    output << "insert";
                } else {
                    output << "Add";
                }
                output << "(" << field_name << ".begin(), " << field_name << ".end());\n";
            } else if (field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE) {
                output << Tabs(1) << "res.set_allocated_" << field_name << "(" << field_name << ");\n";
            } else {
                output << Tabs(1) << "res.set_" << field_name << "(" << field_name << ");\n";
            }
        }
        output << Tabs(1) << "return res;\n";
        output << "}\n\n";
    }
    return output.str();
}

std::string GetIncludeGuard(const grpc::protobuf::FileDescriptor* file) {
    std::string output;
    output = file->name();
    size_t start_pos = 0;
    while ((start_pos = output.find("/", start_pos)) != std::string::npos) {
        output.replace(start_pos, 1, "_");
        start_pos += 1;
    }
    start_pos = 0;
    while ((start_pos = output.find(".", start_pos)) != std::string::npos) {
        output.replace(start_pos, 1, "_");
        start_pos += 1;
    }
    output = "GENERATED_" + output + "_EXT_H";

    return output;
}

// Convert from "a/b/c.proto" to "#include \"a/b/c$message_header_ext$\"\n"
std::string ImportInludeFromProtoName(const std::string& proto_name, const std::string& extension) {
    return std::string("#include \"") +
           proto_name.substr(0, proto_name.size() - 6) +
           extension + "\"\n";
}

std::string GetHeaderCallMethodFunction(const grpc::protobuf::MethodDescriptor* method) {
    std::stringstream output;

    output << "bool Call" << method->name() << "(\n";
    output << Tabs(2) << "const " << StubTypeFromService(method->service()) << " stub,\n";
    output << Tabs(2) << "const " << CppNameFromProtoName(method->input_type()->full_name()) << "& request,\n";
    output << Tabs(2) << CppNameFromProtoName(method->output_type()->full_name()) << "* response);\n";

    return output.str();
}

std::string GetSourceCallMethodFunction(const grpc::protobuf::MethodDescriptor* method) {
    std::stringstream output;

    output << "bool Call" << method->name() << "(\n";
    output << Tabs(2) << "const " << StubTypeFromService(method->service()) << " stub,\n";
    output << Tabs(2) << "const " << CppNameFromProtoName(method->input_type()->full_name()) << "& request,\n";
    output << Tabs(2) << CppNameFromProtoName(method->output_type()->full_name()) << "* response) {\n";

    output << Tabs(1) << "grpc::ClientContext context;\n";
    output << Tabs(1) << "bool result;\n";
    output << Tabs(1) << "std::mutex mu;\n";
    output << Tabs(1) << "std::condition_variable cv;\n";
    output << Tabs(1) << "bool done = false;\n";
    output << Tabs(1) << "stub->async()->" << method->name() << "(\n";
    output << Tabs(2) <<    "&context, &request, response,\n";
    output << Tabs(2) <<    "[&result, &mu, &cv, &done, response](grpc::Status status) {\n";
    output << Tabs(3) <<        "bool ret;";
    output << Tabs(3) <<        "if (!status.ok()) {\n";
    output << Tabs(4) <<            "ret = false;\n";
    output << Tabs(3) <<        "} else {\n";
    output << Tabs(4) <<            "ret = true;\n";
    output << Tabs(3) <<        "}\n";
    output << Tabs(3) <<        "std::lock_guard<std::mutex> lock(mu);\n";
    output << Tabs(3) <<        "result = ret;\n";
    output << Tabs(3) <<        "done = true;\n";
    output << Tabs(3) <<        "cv.notify_one();\n";
    output << Tabs(2) <<    "});\n";
    output << Tabs(1) << "std::unique_lock<std::mutex> lock(mu);\n";
    output << Tabs(1) << "cv.wait(lock, [&done] { return done; });\n";
    output << Tabs(1) << "return result;\n";
    output << "}\n";

    return output.str();
}

std::string GetHeaderPrologue(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << "#ifndef " << GetIncludeGuard(file) << "\n";
    output << "#define " << GetIncludeGuard(file) << "\n\n";

    return output.str();
}

std::string GetHeaderIncludes(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << ImportInludeFromProtoName(file->name(), ".grpc.pb.h");
    for (int i = 0; i < file->dependency_count(); i++) {
        const auto& dep = *file->dependency(i);
        output << ImportInludeFromProtoName(dep.name(), ".grpc-ext.pb.h");
    }
    output << "\n";

    return output.str();
}

std::string GetHeaderServices(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << "namespace " << file->package() << " {\n\n";

    for (size_t service_index = 0; service_index < file->service_count(); service_index++) {
        const google::protobuf::ServiceDescriptor* service = file->service(service_index);

        for (size_t method_index = 0; method_index < service->method_count(); method_index++) {
            const google::protobuf::MethodDescriptor* method = service->method(method_index);

            if (method->client_streaming() || method->server_streaming()) {
                continue;
            }
            output << GetHeaderCallMethodFunction(method) << "\n";
        }
    }

    output << "} // namespace " << file->package() << "\n\n";

    return output.str();
}

std::string GetHeaderEpilogue(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << "#endif // " << GetIncludeGuard(file) << "\n";

    return output.str();
}

std::string GetSourcePrologue(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    return output.str();
}

std::string GetSourceIncludes(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << ImportInludeFromProtoName(file->name(), ".grpc-ext.pb.h");
    output << "\n";
    output << "#include <mutex>\n";
    output << "#include <condition_variable>\n";
    output << "\n";


    return output.str();
}

std::string GetSourceServices(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;

    output << "namespace " << file->package() << " {\n\n";

    for (size_t service_index = 0; service_index < file->service_count(); service_index++) {
        const google::protobuf::ServiceDescriptor* service = file->service(service_index);

        for (size_t method_index = 0; method_index < service->method_count(); method_index++) {
            const google::protobuf::MethodDescriptor* method = service->method(method_index);

            if (method->client_streaming() || method->server_streaming()) {
                continue;
            }
            output << GetSourceCallMethodFunction(method) << "\n";
        }
    }

    output << "} // namespace " << file->package() << "\n\n";

    return output.str();
}

std::string GetSourceEpilogue(const grpc::protobuf::FileDescriptor* file) {
    std::stringstream output;
    return output.str();
}

} // namespace extended_cpp_generator
