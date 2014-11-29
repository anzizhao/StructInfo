typedef struct tagNET_DVR_ALARMINFO_FIXED_HEADER
{
    DWORD dwAlarmType;              /*0-信号量报警,1-硬盘满,2-信号丢失，3－移动侦测，4－硬盘未格式化,5-写硬盘出错,6-遮挡报警，7-制式不匹配, 8-非法访问，9-视频信号异常，10-录像异常，11-智能场景变化，12-阵列异常，13-前端/录像分辨率不匹配*/
    NET_DVR_TIME_EX struAlarmTime;	//发生报警的时间
    union
    {
        BYTE	byUnionLen[128];
        struct
        {
            DWORD	dwAlarmInputNo;		//发生报警的报警输入通道号，一次只有一个
            DWORD	dwTrigerAlarmOutNum;	/*触发的报警输出个数，用于后面计算变长数据部分中所有触发的报警输出通道号，四字节表示一个*/
            DWORD	dwTrigerRecordChanNum;	/*触发的录像通道个数，用于后面计算变长数据部分中所有触发的录像通道号，四字节表示一个*/
        }struIOAlarm;	// 报警类型dwAlarmType为0时有效
        struct
        {
            DWORD	dwAlarmChanNum;	/*发生报警通道数据个数，用于后面计算变长数据部分中所有发生的报警通道号，四字节表示一个*/
        }struAlarmChannel;	// dwAlarmType为2，3，6，9，10或13时有效
        struct
        {
            DWORD	dwAlarmHardDiskNum;	/*发生报警的硬盘数据长度，用于后面计算变长数据部分中所有发生报警的硬盘号，四节表示一个*/
        }struAlarmHardDisk;	// dwAlarmType为1,4,5时有效
		struct
		{
			BYTE       bySubAlarmType;  //报警子类型，1-一键延迟录像； 
			BYTE       byRes1[3]; //保留
			NET_DVR_TIME_EX  struRecordEndTime; //录播结束时间
			BYTE    byRes[116]; //保留
		}struRecordingHost;  //录播主机专用报警
    }uStruAlarm;
}NET_DVR_ALRAM_FIXED_HEADER, *LPNET_DVR_ALARM_FIXED_HEADER;