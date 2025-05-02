// zlang.h - Z语言编译器核心定义头文件
// 版本：1.2.1(Fix)
// 最后更新：2025-5-2
// zlang.h - Z语言编译器核心定义头文件（修正版）
#pragma once
#include <vector>
#include <string>
#include <utility>

namespace zlang {

// 标准库映射
const std::vector<std::pair<std::string, std::string>> STD_LIBRARIES = {
    {"系统", "sys"}, {"操作系统", "os"}, {"数学", "math"},
    {"时间", "time"}, {"随机数", "random"}, {"请求库", "requests"},
    {"异步库", "asyncio"}, {"数据分析库", "pandas"}, 
    {"绘图库", "matplotlib"}, {"科学计算", "numpy"}
};

// 修正后的词法规则（优先级顺序优化）
const std::vector<std::pair<std::string, std::string>> LEX_RULES = {
    // 符号标准化（最高优先级）
    {R"(（)", "("},          {R"(）)", ")"},
    {R"(“)", "\""},         {R"(”)", "\""},
    {R"(「)", "'"},          {R"(」)", "'"},
    {R"(：)", ":"},          {R"(，)", ","},
    {R"(；)", ";"},          {R"(？)", "#"},

    // 类与对象
    {R"(^定义类\s+(\w+)\s*继承\s*(\w+)\s*:\s*)", "class $1($2):\n"},
    {R"(^定义类\s+(\w+)\s*:\s*)", "class $1:\n"},
    {R"(^新建实例\s+(\w+)\s*=\s*)", "$1 = "},
    {R"(自身\.)", "self."},

    // 类型注解
    {R"(: 整数型\b)", ": int"},     {R"(: 浮点型\b)", ": float"},
    {R"(: 文本型\b)", ": str"},     {R"(: 布尔型\b)", ": bool"},
    {R"(: 列表型\b)", ": list"},    {R"(: 字典型\b)", ": dict"},
    {R"(: 元组型\b)", ": tuple"},   {R"(: 空值型\b)", ": None"},
    {R"(-> 整数型\b)", "-> int"},   {R"(-> 浮点型\b)", "-> float"},

    // 异常处理
    {R"(^抛出\s+(\w+)\s*)", "raise $1"},
    {R"(^尝试:\s*)", "try:\n"},
    {R"(^捕获\s+(\w+)\s+为\s+(\w+)\s*:\s*)", "except $1 as $2:\n"},
    {R"(^最终:\s*)", "finally:\n"},

    // 异步支持
    {R"(^异步定义函数\s+)", "async def "},
    {R"(^异步遍历\s+)", "async for "},
    {R"(^等待\s+)", "await "},
    {R"(^异步上下文\s+)", "async with "},

    // 控制结构
    {R"(^当\s+(.+)\s*:\s*)", "while $1:\n"},
    {R"(^遍历区间\s+(\d+)\s+到\s+(\d+)\s+步长\s+(\d+)\s*:\s*)", "for _ in range($1, $2, $3):\n"},
    {R"(^无穷循环:\s*)", "while True:\n"},

    // 基础语法（包含修正的打印规则）
    {R"(^打印\((.*?)\))", "print($1)"},
    {R"(^定义函数\s+)", "def "},
    {R"(^返回\s+)", "return "},
    {R"(^导入\s+)", "import "},
    {R"(^从\s+(\S+)\s+导入\s+(\S+)\s*)", "from $1 import $2"},

    // 运算符
    {R"(\＋)", "+"},         {R"(\－)", "-"},
    {R"(\×)", "*"},          {R"(÷)", "/"},
    {R"(余)", "%"},          {R"(次方)", "**"},
    {R"(全等于)", "=="},      {R"(不等于)", "!="},
    {R"(并且)", " and "},    {R"(或者)", " or "},
    {R"(属于)", " in "},     {R"(不属于)", " not in "},

    // 其他关键字
    {R"(^空操作\s*)", "pass\n"},
    {R"(^中断循环\s*)", "break\n"},
    {R"(^继续循环\s*)", "continue\n"},
    {R"(^如果\s+(.*?)\s*:\s*)", "if $1:\n"},
    {R"(^否则如果\s+(.*?)\s*:\s*)", "elif $1:\n"},
    {R"(^否则\s*:\s*)", "else:\n"},
    {R"(^为每\s+(.*?)\s+在\s+(.*?)\s*:\s*)", "for $1 in $2:\n"}
};

constexpr bool ENABLE_DEBUG = false;
constexpr int TAB_WIDTH = 4;

} // namespace zlang
