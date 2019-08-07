# !/bin/bash

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
		#检测进程是否存在
		pid=`./GuardOperator.sh p 'root' 'ml_engine'`
		echo $pid
		if [ $pid == $ProNotExists ] ; then
			echo "reboot"
		else
			#检测内存使用情况
			mem=`./GuardOperator.sh m 'root' 'ml_engine' 80 80`
			if [ $mem == $MemLarger ] ; then
				kill -9 $pid
				echo "reboot"
			fi

			#检测cpu使用情况
			cpu=`./GuardOperator.sh c 'root' 'ml_engine' 80`
			if [ $cpu == $CpuLarger ] ; then
				kill -9 $pid
				echo "reboot"
			fi
		fi
	done
}
main
