# GRPC Extended CPP Plugin

This plugin is an extension of the official GRPC CPP plugin. It adds the following features:
- Generation of `Create` functions for more convenient object creation.
- Generation of `Call` functions for more convenient synchronized RPC calls.

## Usage

```sh
protoc \
  -I <proto_path> \
  --cpp_out <output_path> \
  --grpc_out <output_path> \
  --plugin=protoc-gen-grpc="<path_to_plugin/grpc-extended-cpp-plugin" \
  --ext_grpc_out <output_path> \
  --plugin=protoc-gen-ext_grpc="<path_to_plugin/grpc-extended-cpp-plugin" \
  <proto_files>
```

## Further Information

This plugin fully depends on the grpc repository. It is required to generate the protoc CPP plugin and official GRPC CPP plugin first.

The GRPC Extended CPP Plugin modifies the `.pb.*` files by adding the `Create` functions. The `Call` functions are added into new `.grpc-ext.pb.*` files. The `.grpc.pb.*` files are not modified.

## Documentation

All documentation and the issue tracking can be found in our main repository here: https://github.com/EVerest/everest
