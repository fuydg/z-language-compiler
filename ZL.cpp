//Version 1.2.1(Fix)
// ZL.cpp（兼容Termux的修正版）
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <filesystem>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include "zlang.h"

namespace fs = std::filesystem;

const std::string HELP_INFO = R"(
Z语言编译器 v1.2.1(Fix)
用法: ZL [选项] <源文件.z>

选项:
  -h, --HELP     显示帮助
  -v, --VERSION  版本信息
  -o <文件>      指定输出文件
  -d, --DEBUG    调试模式

示例:
  ZL demo.z         # 直接执行
  ZL demo.z -o out  # 生成out.py
)";

const std::string VERSION_INFO = "Z语言编译器 1.2.1(Fix)\n编译日期: " __DATE__;

std::string translate_identifier(const std::string& ident) {
    for (const auto& [zh, en] : zlang::STD_LIBRARIES) {
        if (ident == zh) return en;
    }
    return ident;
}

std::string preprocess_code(const std::string& code) {
    std::stringstream ss(code);
    std::string line, result;

    while (std::getline(ss, line)) {
        size_t comment_pos = line.find("#>");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        for (const auto& [zh, en] : zlang::STD_LIBRARIES) {
            size_t pos = 0;
            while ((pos = line.find(zh, pos)) != std::string::npos) {
                line.replace(pos, zh.length(), en);
                pos += en.length();
            }
        }

        if (!line.empty()) result += line + "\n";
    }
    return result;
}

std::string transpile(const std::string& z_code) {
    std::string py_code = preprocess_code(z_code);

    for (const auto& [pattern, replacement] : zlang::LEX_RULES) {
        try {
            std::regex re(pattern);
            py_code = std::regex_replace(py_code, re, replacement);
        } catch (const std::regex_error& e) {
            std::cerr << "正则错误: " << pattern << std::endl;
        }
    }
    return py_code;
}

void process_args(int argc, char* argv[], std::string& input_file,
                  std::string& output_file, bool& debug_mode) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            std::cout << HELP_INFO << std::endl;
            exit(0);
        } else if (arg == "-v" || arg == "--version") {
            std::cout << VERSION_INFO << std::endl;
            exit(0);
        } else if (arg == "-d" || arg == "--debug") {
            debug_mode = true;
        } else if (arg == "-o") {
            if (i + 1 >= argc) {
                std::cerr << "错误：-o 需要文件名" << std::endl;
                exit(1);
            }
            output_file = argv[++i];
        } else if (arg[0] == '-') {
            std::cerr << "未知选项: " << arg << std::endl;
            exit(1);
        } else {
            if (input_file.empty()) {
                input_file = arg;
            }
        }
    }

    if (input_file.empty()) {
        std::cerr << "错误：需要输入文件\n" << HELP_INFO << std::endl;
        exit(1);
    }

    if (!fs::exists(input_file)) {
        std::string alt = input_file;
        if (fs::path(input_file).extension() == ".z")
            alt = input_file.substr(0, input_file.size() - 2) + ".Z";
        else if (fs::path(input_file).extension() == ".Z")
            alt = input_file.substr(0, input_file.size() - 2) + ".z";
        if (fs::exists(alt)) input_file = alt;
    }
}

int main(int argc, char* argv[]) {
    std::string input_file, output_file;
    bool debug_mode = false;

    try {
        process_args(argc, argv, input_file, output_file, debug_mode);

        std::ifstream file(input_file);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << input_file << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string py_code = transpile(buffer.str());

        if (debug_mode) {
            std::cout << "[DEBUG] 转译代码:\n" << py_code << std::endl;
        }

        const fs::path py_path = fs::temp_directory_path() / "zlang_temp.py";

        std::ofstream out(py_path);
        out << "# -*- coding: utf-8 -*-\n" << py_code;
        out.close();

        if (output_file.empty()) {
            fs::permissions(py_path, fs::perms::owner_exec, fs::perm_options::add);
            std::string cmd = "python " + py_path.string();
            return std::system(cmd.c_str());
        } else {
            fs::copy(py_path, output_file, fs::copy_options::overwrite_existing);
            std::cout << "输出文件: " << output_file << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
