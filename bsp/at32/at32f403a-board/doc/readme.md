# rtthread 学习笔记

> https://club.rt-thread.org/ask/article/2460fcd7db4821ae.html

## 通用命令

```shell
scons -j32 //编译

scons --target=mdk5 //mdk工程生成

pkgs --upgrade //在线包更新

scons --pyconfig //ui版本menuconfig


```




## 关于项目工程问题

1.我们修改魔术棒内的项目配置，启用了 “Use MicroLIB” “Browse Information” “GNU extensions” 等等之后，重新生成可能导致之前的修改丢失，可以通过修改 “template.uvprojx” 文件

2.keil内的文件树使用SConscript管理，添加后运行scons --target=mdk5会自动添加到keil工程内







## 关于代码问题

1.mdk使用task_msg_bus组件时，使用rt_slist_for_each_entry会报错，需要在项目内添加

【options】 -- 【C/C++】 -- 【Misc Controls】，增加--gnu

https://club.rt-thread.org/ask/article/2460fcd7db4821ae.html



2.添加spi的flash的sfud设备

https://blog.csdn.net/lengyuefeng212/article/details/113848520

3.添加mtd nor设备

https://blog.csdn.net/weixin_46158019/article/details/143258002

4.添加fal 块设备
注意mtd设备和块设备的区别
https://club.rt-thread.org/ask/article/4101a9965b5058c4.html



## 其他