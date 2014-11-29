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