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


typedef union tagNET_DVR_GET_STREAM_UNION
{
    NET_DVR_IPCHANINFO      struChanInfo;	/*IP通道信息*/
    NET_DVR_IPSERVER_STREAM struIPServerStream;  // IPServer去流
    NET_DVR_PU_STREAM_CFG   struPUStream;     //  通过前端设备获取流媒体去流
	NET_DVR_DDNS_STREAM_CFG struDDNSStream;     //通过IPServer和流媒体取流
	NET_DVR_PU_STREAM_URL   struStreamUrl;        //通过流媒体到url取流
	NET_DVR_HKDDNS_STREAM	struHkDDNSStream;   //通过hiDDNS去取流
	NET_DVR_IPCHANINFO_V40 struIPChan; //直接从设备取流（扩展）
}NET_DVR_GET_STREAM_UNION, *LPNET_DVR_GET_STREAM_UNION;



typedef enum
{
    NET_SDK_IP_DEVICE = 0,
    NET_SDK_STREAM_MEDIA,
    NET_SDK_IPSERVER,
	NET_SDK_DDNS_STREAM_CFG,
	NET_SDK_STREAM_MEDIA_URL,
	NET_SDK_HKDDNS,
    NET_SDK_IP_DEVICE_ADV,
    NET_SDK_IP_DEVICE_V40, 
    NET_SDK_RTSP  
}GET_STREAM_TYPE;



typedef struct tagNET_DVR_STREAM_MODE
{
    BYTE    byGetStreamType; //取流方式GET_STREAM_TYPE，0-直接从设备取流，1-从流媒体取流、2-通过IPServer获得ip地址后取流,3.通过IPServer找到设备，再通过流媒体去设备的流
                           //4-通过流媒体由URL去取流,5-通过hkDDNS取流，6-直接从设备取流(扩展)，使用NET_DVR_IPCHANINFO_V40结构, 7-通过RTSP协议方式进行取流
	BYTE    byRes[3];        //保留字节
    NET_DVR_GET_STREAM_UNION uGetStream;    // 不同取流方式结构体
}NET_DVR_STREAM_MODE, *LPNET_DVR_STREAM_MODE;

//扩展IP接入配置设备
typedef struct tagNET_DVR_IPPARACFG_V40
{
    DWORD      dwSize;			                /* 结构大小 */
    DWORD	   dwGroupNum;					//	 设备支持的总组数    
    DWORD      dwAChanNum;					//最大模拟通道个数
    DWORD      dwDChanNum;                  //数字通道个数
    DWORD      dwStartDChan;		            //起始数字通道
    BYTE       byAnalogChanEnable[MAX_CHANNUM_V30];    /* 模拟通道是否启用，从低到高表示1-64通道，0表示无效 1有效 */
    NET_DVR_IPDEVINFO_V31   struIPDevInfo[MAX_IP_DEVICE_V40];      /* IP设备 */
    NET_DVR_STREAM_MODE  struStreamMode[MAX_CHANNUM_V30];
    BYTE            byRes2[20];                 // 保留字节
}NET_DVR_IPPARACFG_V40, *LPNET_DVR_IPPARACFG_V40;