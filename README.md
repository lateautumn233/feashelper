# Feashelper

#### 介绍
在支持的小米设备上，自动给配置指定的应用打开Feas，未指定的关闭

#### 安装教程

1.下载最新relase
2.在Magisk内安装

#### 使用说明
配置你自己的应用列表
编辑/data/feas.conf

Example:
***********************************************
# 配置格式:
# 每行一个（需要打开feas的应用）的包名且只能有包名
# 配置实时生效，模块更新不会覆盖配置
# 支持简单的注释，用'#'注释该行
# 尽量声明游戏运行帧率，因为系统识别它非常不智能
com.miHoYo.Yuanshen 60
com.hypergryph.arknights 60
com.miHoYo.enterprise.NGHSoD 90
com.shangyoo.neon 60
com.omarea.vtools 120
com.mojang.minecraftpe 120
# com.tencent.mobileqq 120
com.netease.x19 120
***********************************************