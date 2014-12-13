typedef struct tagNET_DVR_ALARMINFO_FIXED_HEADER
{
    DWORD dwAlarmType;              /*0-�ź�������,1-Ӳ����,2-�źŶ�ʧ��3���ƶ���⣬4��Ӳ��δ��ʽ��,5-дӲ�̳���,6-�ڵ�������7-��ʽ��ƥ��, 8-�Ƿ����ʣ�9-��Ƶ�ź��쳣��10-¼���쳣��11-���ܳ����仯��12-�����쳣��13-ǰ��/¼��ֱ��ʲ�ƥ��*/
    NET_DVR_TIME_EX struAlarmTime;	//����������ʱ��
    union
    {
        BYTE	byUnionLen[128];
        struct
        {
            DWORD	dwAlarmInputNo;		//���������ı�������ͨ���ţ�һ��ֻ��һ��
            DWORD	dwTrigerAlarmOutNum;	/*�����ı���������������ں������䳤���ݲ��������д����ı������ͨ���ţ����ֽڱ�ʾһ��*/
            DWORD	dwTrigerRecordChanNum;	/*������¼��ͨ�����������ں������䳤���ݲ��������д�����¼��ͨ���ţ����ֽڱ�ʾһ��*/
        }struIOAlarm;	// ��������dwAlarmTypeΪ0ʱ��Ч
        struct
        {
            DWORD	dwAlarmChanNum;	/*��������ͨ�����ݸ��������ں������䳤���ݲ��������з����ı���ͨ���ţ����ֽڱ�ʾһ��*/
        }struAlarmChannel;	// dwAlarmTypeΪ2��3��6��9��10��13ʱ��Ч
        struct
        {
            DWORD	dwAlarmHardDiskNum;	/*����������Ӳ�����ݳ��ȣ����ں������䳤���ݲ��������з���������Ӳ�̺ţ��Ľڱ�ʾһ��*/
        }struAlarmHardDisk;	// dwAlarmTypeΪ1,4,5ʱ��Ч
		struct
		{
			BYTE       bySubAlarmType;  //���������ͣ�1-һ���ӳ�¼�� 
			BYTE       byRes1[3]; //����
			NET_DVR_TIME_EX  struRecordEndTime; //¼������ʱ��
			BYTE    byRes[116]; //����
		}struRecordingHost;  //¼������ר�ñ���
    }uStruAlarm;
}NET_DVR_ALRAM_FIXED_HEADER, *LPNET_DVR_ALARM_FIXED_HEADER;


typedef union tagNET_DVR_GET_STREAM_UNION
{
    NET_DVR_IPCHANINFO      struChanInfo;	/*IPͨ����Ϣ*/
    NET_DVR_IPSERVER_STREAM struIPServerStream;  // IPServerȥ��
    NET_DVR_PU_STREAM_CFG   struPUStream;     //  ͨ��ǰ���豸��ȡ��ý��ȥ��
	NET_DVR_DDNS_STREAM_CFG struDDNSStream;     //ͨ��IPServer����ý��ȡ��
	NET_DVR_PU_STREAM_URL   struStreamUrl;        //ͨ����ý�嵽urlȡ��
	NET_DVR_HKDDNS_STREAM	struHkDDNSStream;   //ͨ��hiDDNSȥȡ��
	NET_DVR_IPCHANINFO_V40 struIPChan; //ֱ�Ӵ��豸ȡ������չ��
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
    BYTE    byGetStreamType; //ȡ����ʽGET_STREAM_TYPE��0-ֱ�Ӵ��豸ȡ����1-����ý��ȡ����2-ͨ��IPServer���ip��ַ��ȡ��,3.ͨ��IPServer�ҵ��豸����ͨ����ý��ȥ�豸����
                           //4-ͨ����ý����URLȥȡ��,5-ͨ��hkDDNSȡ����6-ֱ�Ӵ��豸ȡ��(��չ)��ʹ��NET_DVR_IPCHANINFO_V40�ṹ, 7-ͨ��RTSPЭ�鷽ʽ����ȡ��
	BYTE    byRes[3];        //�����ֽ�
    NET_DVR_GET_STREAM_UNION uGetStream;    // ��ͬȡ����ʽ�ṹ��
}NET_DVR_STREAM_MODE, *LPNET_DVR_STREAM_MODE;

//��չIP���������豸
typedef struct tagNET_DVR_IPPARACFG_V40
{
    DWORD      dwSize;			                /* �ṹ��С */
    DWORD	   dwGroupNum;					//	 �豸֧�ֵ�������    
    DWORD      dwAChanNum;					//���ģ��ͨ������
    DWORD      dwDChanNum;                  //����ͨ������
    DWORD      dwStartDChan;		            //��ʼ����ͨ��
    BYTE       byAnalogChanEnable[MAX_CHANNUM_V30];    /* ģ��ͨ���Ƿ����ã��ӵ͵��߱�ʾ1-64ͨ����0��ʾ��Ч 1��Ч */
    NET_DVR_IPDEVINFO_V31   struIPDevInfo[MAX_IP_DEVICE_V40];      /* IP�豸 */
    NET_DVR_STREAM_MODE  struStreamMode[MAX_CHANNUM_V30];
    BYTE            byRes2[20];                 // �����ֽ�
}NET_DVR_IPPARACFG_V40, *LPNET_DVR_IPPARACFG_V40;