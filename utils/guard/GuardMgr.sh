# !/bin/bash

PROCESS=ml_engine:ml_gtest
PRO_MEM_APEX=10:23
PRO_VMEM_APEX=1000:1223
PRO_CPU_APX=20:20

#异常信息
Normal="normal"
ArgsInvalid="args_invalid"
ProNotExists="process_not_exists"
CpuLarger="cpu_larger"
MemLarger="mem_larger"


main()
{
	while [ 1 -eq 1 ]
	do
		ret=`./GuardOperator.sh p 'root' 'ml_engine'`
		echo $ret
		if [ $ret == $ProNotExists ] ; then
			./ml_engine.sh
		fi  
	done
}
main
