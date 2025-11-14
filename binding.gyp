{
    "targets": [{
        "target_name": "nops",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "src/main.cpp",
            "src/wincalls.cpp",
            "src/workers.cpp",
            "src/net.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "include"
        ],
        'libraries': [
            "iphlpapi.lib",
            "psapi.lib"
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }]
}