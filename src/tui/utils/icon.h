#pragma once

#include <string>

namespace icon {
   static const std::string default_file = "\ue64e";
   static const std::string dir = "\uf4d3";
   static const std::string empty_dir = "\uf413";
   static const std::string download_dir = "\uf498";
   static const std::string document = "\uf02d";
   static const std::string image = "\ue60d";
   static const std::string archive = "\uf187";
   static const std::string bin = "\ueae8";
   static const std::string git = "\ue702";
   static const std::string c = "\ue61e";
   static const std::string cmake = "\ue673";
   static const std::string cpp = "\ue61d";
   static const std::string h = "\uf0fd";
   static const std::string python = "\ue73c";
   static const std::string javascript = "\ue781";
   static const std::string typescript = "\ue628";
   static const std::string java = "\ue738";
   static const std::string php = "\ue73d";
   static const std::string ruby = "\ue791";
   static const std::string c_sharp = "\ue648";
   static const std::string f_sharp = "\ue65a";
   static const std::string html = "\ue736";
   static const std::string css = "\uf13c";
   static const std::string react = "\ue7ba";
   static const std::string markdow = "\ue609";
   static const std::string golang = "\ue627";
   static const std::string haskell = "\ue777";
   static const std::string scala = "\ue737";
   static const std::string kotlin = "\ue634";
   static const std::string vim = "\ue62b";
   static const std::string rust = "\ue7a8";

 std::string get_icon(std::string name);
}