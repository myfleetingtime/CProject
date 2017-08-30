/* create by bingo*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_ROW 40             /*��Ļ����*/
#define SCR_COL 100             /*��Ļ����*/

/**
 *������Ϣ�����ṹ
 */
typedef struct city_node {
    char city_id[6];              /**< ���б��*/
    char name[10];                /**< ��������*/
    char jiandu_num[15];          /**< �ල�绰*/
    char zixun_num[15];           /**< ��ѯ�绰*/
    struct city_node *next;       /**< ָ����һ���н���ָ��*/
    struct region_node *rnext;    /**< ָ�����ڵ��ָ��*/
} CITY_NODE;

/**
 *������Ϣ�����ṹ
 */
typedef struct region_node {
    char city_id[6];              /**< �������б��*/
    char region_id[6];            /**< �������*/
    char name[10];                /**< ��������*/
    char level[15];               /**< ��������*/
    char address[15];             /**< ������ַ*/
    char price[5];                /**< ��Ʊ�۸�*/
    char opentime[15];            /**< ����ʱ��*/
    struct region_node *next;     /**< ָ����һ��������ָ��*/
    struct spot_node *snext;      /**< ָ�򾰵����ָ��*/
} REGION_NODE;


/**
 *������Ϣ�����ṹ
 */
typedef struct spot_node {
    char city_id[6];              /**< �������б��*/
    char region_id[6];            /**< �����������*/
    char spot_id[6];              /**< ������*/
    char name[10];                /**< ��������*/
    char address[15];             /**< ����λ��*/
    char opentime[15];            /**< ���ʱ��*/
    char feature[255];            /**< ������ɫ*/
    struct spot_node *next;       /**< ָ����һ��������ָ��*/
} SPOT_NODE;

/**
 *����¥��ס��ͳ����Ϣ�����ṹ
 */
typedef struct use_rate_node {
    char dorm_id[5];            	/**< ����¥��*/
    unsigned short total_num;     	/**< ��סѧ����*/
    unsigned short used_num;     	/**< ��סѧ����*/
    unsigned short free_num;     	/**< �մ�λ��*/
    float use_rate;              	/**< ��ס��*/
    struct use_rate_node *next;   	/**< ָ����һ����ָ��*/
} USE_RATE_NODE;

/**
 *��סѧ������ͳ����Ϣ�����ṹ
 */
typedef struct stu_type_node {
    char grade[5];               	/**< �꼶*/
    unsigned short school_boy1;   	/**< ��ר������*/
    unsigned short school_girl1;   	/**< Ůר������*/
    unsigned short school_boy2;   	/**< �б�������*/
    unsigned short school_girl2;   	/**< Ů��������*/
    unsigned short school_boy3;   	/**< ��˶ʿ����*/
    unsigned short school_girl3;   	/**< Ů˶ʿ����*/
    unsigned short school_boy4;   	/**< �в�ʿ����*/
    unsigned short school_girl4;   	/**< Ů��ʿ����*/
    unsigned short school_boy5;   	/**< ����������*/
    unsigned short school_girl5;   	/**< ����Ů����*/
    struct stu_type_node *next;    	/**< ָ����һ����ָ��*/
} STU_TYPE_NODE;

/**
 *����¥���ס�޷�����ͳ����Ϣ�����ṹ
 */
typedef struct income_node {
    char dorm_id[5];            	/**< ����¥��*/
    float year1_income;          	/**< ���1����*/
    float year2_income;          	/**< ���2����*/
    float year3_income;          	/**< ���3����*/
    float year4_income;          	/**< ���4����*/
    struct income_node *next;     	/**< ָ����һ����ָ��*/
} INCOME_NODE;

/**
 *��סѧ��Ƿ��ͳ����Ϣ�����ṹ
 */
typedef struct uncharge_node {
    char stu_id[12];             	/**< ѧ��*/
    char name[10];             	    /**< ����*/
    char dorm_id[5];            	/**< ����¥��*/
    char room_num[5];               /**< �����*/
    float amount;               	/**< Ƿ�ɽ��*/
    struct uncharge_node *next;   	/**< ָ����һ����ָ��*/
} UNCHARGE_NODE;

/**
 *��Ļ������Ϣ�������ṹ
 */
typedef struct layer_node {
    char LayerNo;            /**< �������ڲ���*/
    SMALL_RECT rcArea;       /**< ����������������*/
    CHAR_INFO *pContent;     /**< �������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;           /**< �������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next; /**< ָ����һ����ָ��*/
} LAYER_NODE;

/**
 *��ǩ���ṹ
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< ��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;           /**< ��ǩ��λ�����׵�ַ*/
    int num;               /**< ��ǩ����*/
} LABEL_BUNDLE;

/**
 *�����ṹ
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< ������λ�����׵�ַ*/
    char *pSort;           /**< �������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;            /**< ������������׵�ַ*/
    int num;               /**< ��������*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*����������Ϣ����ͷ*/
CITY_NODE *gp_head = NULL;                     /*����ͷָ��*/

char *gp_sys_name = "������Ϣ����ϵͳ";    /*ϵͳ����*/

char *gp_city_info_filename = "city.dat";      /*������Ϣ�����ļ�*/
char *gp_region_info_filename = "region.dat";  /*������Ϣ�����ļ�*/
char *gp_spot_info_filename = "spot.dat";      /*������Ϣ�����ļ�*/

char *ga_main_menu[] = {"�ļ�(F)",             /*ϵͳ���˵���*/
                        "����ά��(M)",
                        "���ݲ�ѯ(Q)",
                        "����ͳ��(S)",
                        "����(H)"
};

char *ga_sub_menu[] = {"[S] ���ݱ���",          /*ϵͳ�Ӳ˵���*/
                       "[B] ���ݱ���",
                       "[R] ���ݻָ�",
                       "[X] �˳�    Alt+X",
        //
                       "[S] ������Ϣ",
                       "[T] ������Ϣ",
                       "[D] ������Ϣ",
        //
                       "[S] ������Ϣ",
                       "[T] ������Ϣ",
                       "[D] ������Ϣ",
        //
                       "[I] ��㿴��",
                       "[T] ���о���ͳ��",
        //
                       "[T] ��������",
                       "",
                       "[A] ����..."
};

int ga_sub_menu_count[] = {4, 3, 3, 2, 3};  /*�����˵������Ӳ˵��ĸ���*/
int gi_sel_menu = 1;                        /*��ѡ�е����˵����,��ʼΪ1*/
int gi_sel_sub_menu = 0;                    /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char *gp_scr_att = NULL;    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
char gc_sys_state = '\0';   /*��������ϵͳ״̬���ַ�*/

HANDLE gh_std_out;          /*��׼����豸���*/
HANDLE gh_std_in;           /*��׼�����豸���*/

int LoadCode(char *filename, char **ppbuffer);  /*��������*/
int CreatList(CITY_NODE **pphead);              /*���������ʼ��*/
void InitInterface(void);                       /*ϵͳ�����ʼ��*/
void ClearScreen(void);                         /*����*/
void ShowMenu(void);                            /*��ʾ�˵���*/
void PopMenu(int num);                          /*��ʾ�����˵�*/
void PopPrompt(int num);                        /*��ʾ��������*/
void PopUp(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *);  /*����������Ļ��Ϣά��*/
void PopOff(void);                              /*�رն��㵯������*/
void DrawBox(SMALL_RECT *parea);                /*���Ʊ߿�*/
void LocSubMenu(int num, SMALL_RECT *parea);    /*���˵������˵���λ*/
void ShowState(void);                           /*��ʾ״̬��*/
void TagMainMenu(int num);                      /*��Ǳ�ѡ�е����˵���*/
void TagSubMenu(int num);                       /*��Ǳ�ѡ�е��Ӳ˵���*/
int DealConInput(HOT_AREA *phot_area, int *pihot_num);  /*����̨���봦��*/
int DealInput2(HOT_AREA *pHotArea, int *piHot, char **ppcondition);
void SetHotPoint(HOT_AREA *phot_area, int hot_num);     /*��������*/
void RunSys(CITY_NODE **pphd);                  /*ϵͳ����ģ���ѡ�������*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*����ģ��ĵ���*/
void CloseSys(CITY_NODE *phd);                  /*�˳�ϵͳ*/
BOOL ShowModule(char **pString, int n);

BOOL LoadData(void);           /*���ݼ���*/
BOOL SaveData(void);           /*��������*/
BOOL BackupData(void);         /*��������*/
BOOL RestoreData(void);        /*�ָ�����*/
BOOL ExitSys(void);            /*�˳�ϵͳ*/
BOOL HelpTopic(void);          /*��������*/
BOOL AboutDorm(void);          /*����ϵͳ*/

BOOL MaintainCityInfo(void);           /*ά��������Ϣ*/
BOOL MaintainScenicAreaInfo(void);    /*ά������������Ϣ*/
BOOL MaintainAttractionInfo(void);   /*ά�����������Ϣ*/

BOOL QueryCityInfo(void);           /*��ѯ������Ϣ*/
BOOL QueryScenicAreaInfo(void);    /*��ѯ����������Ϣ*/
BOOL QueryAttractionInfo(void);   /*��ѯ���������Ϣ*/

BOOL RandomView(void);    /*��ѯ����������Ϣ*/
BOOL StatusCityInfo(void);   /*��ѯ���������Ϣ*/

BOOL MaintainSexCode(void);    /*ά���Ա����*/
BOOL MaintainTypeCode(void);   /*ά��ѧ��������*/
BOOL MaintainDormInfo(void);   /*ά������¥��Ϣ*/
BOOL MaintainStuInfo(void);    /*ά��ѧ��������Ϣ*/
BOOL MaintainChargeInfo(void); /*ά��ѧ���ɷ���Ϣ*/

BOOL QuerySexCode(void);       /*��ѯ�Ա����*/
BOOL QueryTypeCode(void);      /*��ѯѧ��������*/
BOOL QueryDormInfo(void);      /*��ѯ����¥��Ϣ*/
BOOL QueryStuInfo(void);       /*��ѯѧ��������Ϣ*/
BOOL QueryChargeInfo(void);    /*��ѯ�ɷ���Ϣ*/

BOOL StatUsedRate(void);       /*ͳ����ס��*/
BOOL StatStuType(void);        /*ѧ������ͳ��*/
BOOL StatCharge(void);         /*ѧ���ɷ���Ϣͳ��*/
BOOL StatUncharge(void);       /*ѧ��Ƿ����Ϣͳ��*/

BOOL InsertChargeNode(CITY_NODE *phd, SPOT_NODE *pspot_node);/*����ɷ���Ϣ���*/
BOOL DelChargeNode(CITY_NODE *phd, char *stu_id, char *date);/*ɾ���ɷ���Ϣ���*/
BOOL ModifChargeNode(CITY_NODE *phd, char *stu_id, char *date, SPOT_NODE *pspot_node);/*�޸Ľɷ���Ϣ���*/
REGION_NODE *SeekStuNode(CITY_NODE *phd, char *stu_id);  /*����ѧ��������Ϣ���*/
SPOT_NODE *SeekChargeNode(CITY_NODE *phd, char *stu_id, char *date);/*���ҽɷ���Ϣ���*/
REGION_NODE *SeekStuNodeM (CITY_NODE *phd, int cond_num, ...);/*ģ����ѯѧ����Ϣ���*/
BOOL JudgeStuNodeItem(REGION_NODE *pregion_node, char *pcond);/*�ж�ѧ����Ϣ�������Ƿ������ѯ����*/
BOOL MatchString(char *string_item, char *pcond);/*�ַ������ѯ�����Ƿ�����ָ���������ϵ*/
BOOL MatchChar(char char_item, char *pcond);/*�ַ����ѯ�����Ƿ�����ָ���������ϵ*/
UNCHARGE_NODE *StatUnchargeInfo(CITY_NODE *phd);        /*ѧ��Ƿ����Ϣͳ��*/
void SortUnchargeInfo(UNCHARGE_NODE *puncharge_hd);     /*Ƿ����Ϣ������*/
BOOL SaveSysData(CITY_NODE *phd);                       /*����ϵͳ����*/
BOOL BackupSysData(CITY_NODE *phd, char *filename);     /*����ϵͳ����*/
BOOL RestoreSysData(CITY_NODE **pphd, char *filename);  /*�ָ�ϵͳ����*/

#endif /**< TYPE_H_INCLUDED*/
