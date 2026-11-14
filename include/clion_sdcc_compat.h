#ifndef CLION_SDCC_COMPAT_H
#define CLION_SDCC_COMPAT_H

/*
 * 仅供 CLion/clangd 的宿主 C 索引目标使用。
 * SDCC 固件构建不会包含本文件。
 */
#define __sfr  volatile unsigned char
#define __sbit volatile unsigned char
#define __at(address)

#endif /* CLION_SDCC_COMPAT_H */
