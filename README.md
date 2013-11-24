jsm-c
=====

See [jsm](http://www.github.com/deoxxa/jsm). This is the JSM virtual machine,
written in C for lulz.

Building
--------

```
➜  jsm-c  make
cc    -c -o execute.o execute.c
cc    -c -o cpu.o cpu.c
cc   execute.o cpu.o   -o execute
```

License
-------

3-clause BSD. A copy is included with the source.

Contact
-------

* GitHub ([deoxxa](http://github.com/deoxxa))
* Twitter ([@deoxxa](http://twitter.com/deoxxa))
* Email ([deoxxa@fknsrs.biz](mailto:deoxxa@fknsrs.biz))
