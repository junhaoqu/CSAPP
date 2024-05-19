``` First terminal```

qemu-x86_64-static -g 1234 ./bomb ./sol.txt

```Second terminal```

(gdb) target remote :1234
(gdb) continue

```reference link```
https://zhuanlan.zhihu.com/p/104130161
https://wdxtub.com/csapp/thick-csapp-lab-2/2016/04/16/
https://www.bilibili.com/video/BV1vu411o7QP?p=7&vd_source=144fa76fd164c884543d73e373dea3d5
