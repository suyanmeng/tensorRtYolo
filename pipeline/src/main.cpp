#include <iostream>

#include "pipeline.h"

void parseArguments(int argc, char** argv, std::string& engine_path,
                    std::string& imgs_dir, std::string& video_path,
                    std::string& output_dir) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " -e <engine_path> (-i <imgs_dir> | -v <video_path>) -o <output_dir> "
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-e" || arg == "--engine") {
            engine_path = argv[++i];
        } else if (arg == "-i" || arg == "--images") {
            imgs_dir = argv[++i];
        } else if (arg == "-v" || arg == "--video") {
            video_path = argv[++i];
        } else if (arg == "-o" || arg == "--output") {
            output_dir = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char** argv) {
    std::string engine_path, imgs_dir, video_path, output_dir;
    parseArguments(argc, argv, engine_path, imgs_dir, video_path, output_dir);
    TensorRTYolo::Pipeline pipeline(engine_path);
    if (imgs_dir.empty() && !video_path.empty()) {
        pipeline.setVideoPath(video_path, output_dir);
    } else if (!imgs_dir.empty() && video_path.empty()) {
        pipeline.setImageDir(imgs_dir, output_dir);
    } else {
        std::cerr
            << "Please specify either a picture directory or a video file."
            << std::endl;
        return EXIT_FAILURE;
    }
    pipeline.run();
    return 0;
}