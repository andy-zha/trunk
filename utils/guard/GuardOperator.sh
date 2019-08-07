# !/bin/bash

#加载运行参数
Opt=$#
OptOne=$1
OptTwo=$2
OptThree=$3
OptFour=$4
OptFive=$5

#异常信息
Normal="normal"
ArgsInvalid="args_invalid"
ProNotExists="process_not_exists"
CpuLarger="cpu_larger"
MemLarger="mem_larger"


#获取进程pid
function GetPid() #PsUser #PsName
{
	pid=-1

	#入参异常检测
	if [ 2 -ne $# ] ; then
		echo $ArgsInvalid 
		return
	else
		PsUser=$1
		PsName=$2
	fi

	#获取进程pid
	pid=`ps -u $PsUser | grep $PsName | grep -v grep | grep -v vi | grep -v dbx\n | grep -v tail | grep -v start | grep -v stop | sed -n 1p | awk '{print $1}'`

	#判断进程是否存在
	if [ "-$pid" == "-" ] ; then
		echo $ProNotExists
		return
	fi

	echo ${pid}
}

#获取进程cpu资源占比
function GetCpu() #Pid
{
	cpu=0
	if [ 1 -ne $# ] ; then
		echo $ArgsInvalid
		return
	else
		Pid=$1
	fi

	cpu=`ps -p $Pid -o pcpu | grep -v CPU | awk '{print $1}' | awk -F . '{print $1}'`
	echo ${cpu}
}

#获取进程内存资源占比
function GetMem() #Pid
{
	mem=0
	if [ 1 -ne $# ] ; then
		echo $ArgsInvalid
		return
	else
		Pid=$1
	fi

	mem=`ps -p $Pid -o pmem | grep -v MEM | awk '{print $1}' | awk -F . '{print $1}'`
	echo ${mem}
}

#获取进程虚存资源占比
function GetVirMem() #Pid
{
	mem=0
	if [ 1 -ne $# ] ; then
		echo $ArgsInvalid
		return
	else
		Pid=$1
	fi

	mem=`ps -o vsz -p $Pid | grep -v VSZ` / 1000
	echo ${mem}
}

#检测cpu资源
function CheckCpu() #PsUser #PsName #Apex
{
	Pid=-1
	Cpu=0
	Apex=$3

	#参数异常检测
	if [ 3 -ne $# ] ; then
		echo $ArgsInvalid
		return
	else
		PsUser=$1
		PsName=$2
	fi

	#获取进程id
	Pid=`GetPid $PsUser $PsName`
	if [ $Pid == $ProNotExists ] ; then
		echo $ProNotExists
		return
	fi

	#获取进程cpu资源占比并进行检测
	Cpu=`GetCpu $Pid`
	if [ $Cpu -gt $Apex ] ; then
		echo $CpuLarger
		return
	fi
	
	echo $Normal 
}

#检测内存资源
function CheckMem() #PsUser #PsName #Apex #VApex
{
	Pid=-1
	Mem=0
	Apex=$3
	VApex=$4

	#参数异常检测
	if [ 4 -ne $# ] ; then
		echo $ArgsInvalid
		return
	else
		PsUser=$1
		PsName=$2
	fi

	#获取进程id
	Pid=`GetPid $PsUser $PsName`
	if [ $Pid == $ProNotExists ] ; then
		echo $ProNotExists
		return
	fi

	#获取进程内存资源占比并进行检测
	Mem=`GetMem $Pid`
	if [ $Mem -gt $Apex ] ; then
		echo $MemLarger
		return
	fi

	#虚拟内存自动占比检测
	VMem=`GetVirMem $Pid`
	if [ $VMem -gt $VApex ] ; then
		echo $MemLarger
		return
	fi
	
	echo $Normal 
}

#主程序入口
main()
{
	#参数判断
	if [ 2 -gt $Opt ] ; then
		echo $ArgsInvalid 
		return 
	fi
 
	case $OptOne in
		#获取进程id
		p)
			#获取进程id参数应为3个
			if [ 3 -ne $Opt ] ; then
				echo $ArgsInvalid
				return
			fi

			#获取进程id
			ret=`GetPid $OptTwo $OptThree`
			#判断进程是否存在
			if [ $ret == $ProNotExists ] ; then
				echo $ProNotExists
				return
			else
				echo $ret
				return
			fi
		;;
		#cpu检测	
		c)
			#cpu检测参数应为4个
			if [ 4 -ne $Opt ] ; then
				echo $ArgsInvalid
				return
			fi

			#cpu资源检测
			ret=`CheckCpu $OptTwo $OptThree $OptFour`
			echo $ret
			#进程存在判断
			if [ $ret == $ProNotExists ] ; then
				echo $ProNotExists 
				return
			#cpu资源超过检测阈值
			elif [ $ret == $CpuLarger ] ; then
				echo $CpuLarger
				return
			fi
		;;
		#内存检测
		m)
			#内存检测参数应该为5个
			if [ 5 -ne $Opt ] ; then
				echo $ArgsInvalid
				return
			fi
		
			#内存资源检测	
			ret=`CheckMem $OptTwo $OptThree $OptFour $OptFive`
			#进程存在判断
			if [ $ret == $ProNotExists ] ; then
				echo $ProNotExists
				return
			#内存资源超过检测阈值
			elif [ $ret == $MemLarger ] ; then
				echo $MemLarger
				return
		fi
		;;
		*)
			echo $ArgsInvalid
			return
		;;
	esac

	echo $Normal
}
main
