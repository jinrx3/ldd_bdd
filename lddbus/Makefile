ifeq ($(PARAM_FILE), )
		PARAM_FILE:=../../linksdk/smp/a7_linux/mpp/Makefile.param
		include $(PARAM_FILE)
endif

obj-m := lddbus.o ldd_drv.o ldd_dev.o
ldd-y += lddbus.o ldd_drv.o ldd_dev.o


EXTRA_CFLAGS+=-I$(PWD)/

default:	

	@$(MAKE) -C $(KERNEL_ROOT) M=$(PWD) modules 

clean:
	@make -C $(KERNEL_ROOT) M=$(PWD) clean
