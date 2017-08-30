#include "dorm.h"

unsigned long ul;

int main()
{
    COORD size = {SCR_COL, SCR_ROW};              /*���ڻ�������С*/

    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* ��ȡ��׼����豸���*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* ��ȡ��׼�����豸���*/

    SetConsoleTitle(gp_sys_name);                 /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*���ô��ڻ�������С80*25*/

    LoadData();                   /*���ݼ���*/
    InitInterface();          /*�����ʼ��*/
    RunSys(&gp_head);             /*ϵͳ����ģ���ѡ������*/
    CloseSys(gp_head);            /*�˳�ϵͳ*/

    return 0;
}

/**
 * ��������: LoadData
 * ��������: ������������������ݴ������ļ����뵽�ڴ滺������ʮ��������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ���ܺ����г��˺���ExitSys�ķ���ֵ����ΪFALSE��,
 *           ���������ķ���ֵ����ΪTRUE.
 *
 * ����˵��: Ϊ���ܹ���ͳһ�ķ�ʽ���ø����ܺ���, ����Щ���ܺ�����ԭ����Ϊ
 *           һ��, ���޲����ҷ���ֵΪBOOL. ����ֵΪFALSEʱ, ������������.
 */
BOOL LoadData()
{
    int Re = 0;
    Re = CreatList(&gp_head);
    gc_sys_state |= Re;
    gc_sys_state &= ~(4 + 8 + 16 - Re);
    if (gc_sys_state < (1 | 2 | 4 | 8 | 16))
    {  /*���ݼ�����ʾ��Ϣ*/
        printf("\nϵͳ�������ݲ�����!\n");
        printf("\n�����������...\n");
        getch();
    }

    return TRUE;
}

/**
 * ��������: LoadCode
 * ��������: �������������ļ����뵽�ڴ滺����, �����������ȥ���ո�.
 * �������: FileName ��Ŵ����������ļ���.
 * �������: pBuffer ָ���ڴ滺������ָ������ĵ�ַ.
 * �� �� ֵ: ��Ŵ������ڴ滺������С(���ֽ�Ϊ��λ).
 *
 * ����˵��:
 */
int LoadCode(char *FileName, char **pBuffer)
{
    char *pTemp, *pStr1, *pStr2;
    int handle;
    int BufferLen, len, loc1, loc2, i;
    long filelen;

    if ((handle = open(FileName, O_RDONLY | O_TEXT)) == -1) /*�����ֻ����ʽ��ʧ�� */
    {
        handle = open(FileName, O_CREAT | O_TEXT, S_IREAD); /*�Դ�����ʽ��*/
    }
    filelen = filelength(handle);      /*�����ļ��ĳ���*/
    pTemp = (char *)calloc(filelen + 1, sizeof(char)); /*����ͬ����С�Ķ�̬�洢��*/
    BufferLen = read(handle, pTemp, filelen); /*�������ļ�������ȫ�����뵽�ڴ�*/
    close(handle);

    *(pTemp + BufferLen) = '\0'; /*�ڶ�̬�洢��β��һ�����ַ�����Ϊ�ַ���������־*/
    BufferLen++;

    for (i=0; i<BufferLen; i++) /*����̬�洢���е����л��з��滻�ɿ��ַ�*/
    {
        if (*(pTemp + i) == '\n')
        {
            *(pTemp + i) = '\0';
        }
    }

    /*������һ��ͬ����С�Ķ�̬�洢�������ڴ�������Ĵ��봮*/
    *pBuffer = (char *)calloc(BufferLen, sizeof(char));
    loc2 = 0;
    pStr1 = pTemp;
    len = strlen(pStr1);

    while (BufferLen > len + 1) /*ѡ������*/
    {
        loc1 = len + 1;
        while (BufferLen > loc1) /*ÿ���ҵ���������С���봮���׵�ַ����pStr1*/
        {
            pStr2 = pTemp + loc1;
            if (strcmp(pStr1, pStr2) > 0)
            {
                pStr1 = pStr2;
            }
            loc1 += strlen(pStr2) + 1;
        }
        len = strlen(pStr1);  /*��һ�����ҵ�����С���봮����*/

        /*������ǿմ�������и��ƣ�loc2����һ����С���봮��ŵ�ַ��ƫ����*/
        if (len > 0)
        {
            strcpy(*pBuffer + loc2, pStr1);
            loc2 += len + 1;  /*�Ѹ��ƵĴ��봮��ռ�洢�ռ��С*/
        }

        /*����С���봮��������ɾ����*/
        for(i=0; i<BufferLen-(pStr1-pTemp)-(len+1); i++)
        {
            *(pStr1 + i) = *(pStr1 + i + len + 1);
        }

        BufferLen -= len + 1; /*��һ���������������еĳ���*/
        pStr1 = pTemp;  /*�ٶ����еĵ�һ�����봮Ϊ��С���봮*/
        len = strlen(pStr1);
    } /*������ֻʣ��һ�����봮ʱ���������*/

    /*�������������봮*/
    len = strlen(pStr1);
    strcpy(*pBuffer + loc2, pStr1);

    /*�޸Ķ�̬�洢����С��ʹ�����÷����������봮*/
    loc2 += len + 1;
    *pBuffer = (char *)realloc(*pBuffer, loc2);
    free(pTemp);  /*�ͷ���������Ķ�̬�洢��*/

    return loc2;  /*���ش�Ŵ��봮���ڴ滺����ʵ�ʴ�С*/
}

/**
 * ��������: CreatList
 * ��������: �������ļ���ȡ��������, ����ŵ���������ʮ��������.
 * �������: ��
 * �������: phead ����ͷָ��ĵ�ַ, ����������������ʮ����.
 * �� �� ֵ: int����ֵ, ��ʾ�����������.
 *           0  ����, ������
 *           4  �Ѽ�������¥��Ϣ���ݣ��޾���������Ϣ�ͽɷ���Ϣ����
 *           12 �Ѽ�������¥��Ϣ�;���������Ϣ���ݣ��޽ɷ���Ϣ����
 *           28 ����������ݶ��Ѽ���
 *
 * ����˵��:
 */
int CreatList(CITY_NODE **phead)
{
    CITY_NODE *hd = NULL, *pCityNode, tmp1;
    REGION_NODE *pRegionNode, tmp2;
    SPOT_NODE *pSpotNode, tmp3;
    FILE *pFile;
    int find;
    int re = 0;

    if ((pFile = fopen(gp_city_info_filename, "rb")) == NULL)
    {
        printf("������Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("������Ϣ�����ļ��򿪳ɹ�!\n");

    /*�������ļ��ж�������Ϣ���ݣ������Ժ���ȳ���ʽ������������*/
    while (fread(&tmp1, sizeof(CITY_NODE), 1, pFile) == 1)
    {
        pCityNode = (CITY_NODE *)malloc(sizeof(CITY_NODE));
        *pCityNode = tmp1;
        pCityNode->rnext = NULL;
        pCityNode->next = hd;
        hd = pCityNode;
    }
    fclose(pFile);
    if (hd == NULL)
    {
        printf("������Ϣ�����ļ�����ʧ��!\n");
        return re;
    }
    printf("������Ϣ�����ļ����سɹ�!\n");
    *phead = hd;
    re += 4;

    if ((pFile = fopen(gp_region_info_filename, "rb")) == NULL)
    {
        printf("����������Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("����������Ϣ�����ļ��򿪳ɹ�!\n");
    re += 8;

    /*�������ļ��ж�ȡ����������Ϣ���ݣ�����������Ӧ���ľ���������Ϣ֧����*/
    while (fread(&tmp2, sizeof(REGION_NODE), 1, pFile) == 1)
    {
        /*������㣬��Ŵ������ļ��ж����ľ���������Ϣ*/
        pRegionNode = (REGION_NODE *)malloc(sizeof(REGION_NODE));
        *pRegionNode = tmp2;
        pRegionNode->snext = NULL;

        /*�������ϲ��Ҹþ����������ж�Ӧ���������*/
        pCityNode = hd;
        while (pCityNode != NULL
               && strcmp(pCityNode->city_id, pRegionNode->city_id) != 0)
        {
            pCityNode = pCityNode->next;
        }
        if (pCityNode != NULL) /*����ҵ����򽫽���Ժ���ȳ���ʽ���뾰����Ϣ֧��*/
        {
            pRegionNode->next = pCityNode->rnext;
            pCityNode->rnext = pRegionNode;
        }
        else  /*���δ�ҵ������ͷ������������ڴ�ռ�*/
        {
            free(pRegionNode);
        }
    }
    fclose(pFile);

    if ((pFile = fopen(gp_spot_info_filename, "rb")) == NULL)
    {
        printf("������Ϣ�����ļ���ʧ��!\n");
        return re;
    }
    printf("������Ϣ�����ļ��򿪳ɹ�!\n");
    re += 16;

    /*�������ļ��ж�ȡ������Ϣ���ݣ����뾰��������Ϣ֧����Ӧ���ľ���֧����*/
    while (fread(&tmp3, sizeof(SPOT_NODE), 1, pFile) == 1)
    {
        /*������㣬��Ŵ������ļ��ж����ľ����ɷ���Ϣ*/
        pSpotNode = (SPOT_NODE *)malloc(sizeof(SPOT_NODE));
        *pSpotNode = tmp3;

        /*���Ҿ�����Ϣ֧���϶�Ӧ������Ϣ���*/
        pCityNode = hd;
        find = 0;
        while (pCityNode != NULL && find == 0)
        {
            pRegionNode = pCityNode->rnext;
            while (pRegionNode != NULL && find == 0)
            {
                if (strcmp(pRegionNode->region_id, pSpotNode->region_id) == 0)
                {
                    find = 1;
                    break;
                }
                pRegionNode = pRegionNode->next;
            }
            pCityNode = pCityNode->next;
        }
        if (find)  /*����ҵ����򽫽���Ժ���ȳ���ʽ���뾰���ɷ���Ϣ֧����*/
        {
            pSpotNode->next = pRegionNode->snext;
            pRegionNode->snext = pSpotNode;
        }
        else /*���δ�ҵ������ͷ������������ڴ�ռ�*/
        {
            free(pSpotNode);
        }
    }
    fclose(pFile);

    return re;
}

/**
 * ��������: InitInterface
 * ��������: ��ʼ������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*��ɫǰ������ɫ����*/

    SetConsoleTextAttribute(gh_std_out, att);  /*���ÿ���̨��Ļ�������ַ�����*/

    ClearScreen();  /* ����*/

    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;

    ShowMenu();     /*��ʾ�˵���*/
    ShowState();    /*��ʾ״̬��*/

    return;
}

/**
 * ��������: ClearScreen
 * ��������: �����Ļ��Ϣ.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 0};
    unsigned long size;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*ȡ��Ļ��������Ϣ*/
    size =  bInfo.dwSize.X * bInfo.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/

    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);

    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);

    return;
}

/**
 * ��������: ShowMenu
 * ��������: ����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ShowMenu()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur;
    COORD size;
    COORD pos = {0, 0};
    int i, j;
    int PosA = 2, PosB;
    char ch;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos);
    for (i=0; i < 5; i++) /*�ڴ��ڵ�һ�е�һ�д�������˵���*/
    {
        printf("  %s  ", ga_main_menu[i]);
    }

    GetConsoleCursorInfo(gh_std_out, &lpCur);
    lpCur.bVisible = FALSE;
    SetConsoleCursorInfo(gh_std_out, &lpCur);  /*���ع��*/

    /*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0} ;

    /*�����ڵ�һ�е����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    /*����һ����Ӣ����ĸ��Ϊ��ɫ�������ַ���Ԫ��Ϊ�׵׺���*/
    for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*�����λ�������ڵ�2�е�1��*/

    /*���˵�����Ϊ�������������Ϊ�˵���ţ���������Ϊ0(��ť��)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*��λ��i+1�Ų˵������ֹλ��*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*���ò˵��������ַ���Ԫ������ֵ*/
        }
        PosA = PosB + 4;
        i++;
    } while (i<5);

    TagMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/

    return;
}

/**
 * ��������: ShowState
 * ��������: ��ʾ״̬��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��: ״̬���ַ�����Ϊ�׵׺���, ��ʼ״̬��״̬��Ϣ.
 */
void ShowState()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int i;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SMALL_RECT rcMenu ={0, bInfo.dwSize.Y-1, size.X-1, bInfo.dwSize.Y-1};

    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }

    for (i=0; i<size.X; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                | BACKGROUND_RED;
/*
        ch = (char)((gp_buff_stateBar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_stateBar_info+i)->Attributes |= FOREGROUND_RED;
        }
*/
    }

    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);

    return;
}

/**
 * ��������: TagMainMenu
 * ��������: ��ָ�����˵�������ѡ�б�־.
 * �������: num ѡ�е����˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    char ch;
    int i;

    if (num == 0) /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;

    /*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*��ѡ�в˵���������ǣ��ڵװ���*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }

    /*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    return;
}

/**
 * ��������: CloseSys
 * ��������: �ر�ϵͳ.
 * �������: hd ����ͷָ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void CloseSys(CITY_NODE *hd)
{
    CITY_NODE *pCityNode1 = hd, *pCityNode2;
    REGION_NODE *pRegionNode1, *pRegionNode2;
    SPOT_NODE *pSpotNode1, *pSpotNode2;

    while (pCityNode1 != NULL) /*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
    {
        pCityNode2 = pCityNode1->next;
        pRegionNode1 = pCityNode1->rnext;
        while (pRegionNode1 != NULL) /*�ͷž���������Ϣ֧���Ķ�̬�洢��*/
        {
            pRegionNode2 = pRegionNode1->next;
            pSpotNode1 = pRegionNode1->snext;
            while (pSpotNode1 != NULL) /*�ͷŽɷ���Ϣ֧���Ķ�̬�洢��*/
            {
                pSpotNode2 = pSpotNode1->next;
                free(pSpotNode1);
                pSpotNode1 = pSpotNode2;
            }
            free(pRegionNode1);
            pRegionNode1 = pRegionNode2;
        }
        free(pCityNode1);  /*�ͷ��������Ķ�̬�洢��*/
        pCityNode1 = pCityNode2;
    }

    ClearScreen();        /*����*/

    /*�ͷŴ�Ų˵�����״̬�����Ա����;������������Ϣ��̬�洢��*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);

    /*�رձ�׼���������豸���*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);

    /*�����ڱ�������Ϊ���н���*/
    SetConsoleTitle("���н���");

    return;
}

/**
 * ��������: RunSys
 * ��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
 * �������: ��
 * �������: phead ����ͷָ��ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void RunSys(CITY_NODE **phead)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    while (bRet)
    {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);

        if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
            if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
            {
                /* cAtt > 0 ������λ�ô�������(���˵����ַ���Ԫ)
                 * cAtt != gi_sel_menu ������λ�õ����˵���δ��ѡ��
                 * gp_top_layer->LayerNo > 0 ������ǰ���Ӳ˵�����
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
                }
            }
            else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
            {
                TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*������������ߵ�һ��*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(���˵����ַ���Ԫ)*/
                    {
                        PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
                    }
                        /*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    }
                }
                else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
                    {
                        PopOff(); /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/

                        /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/

            /*ϵͳ��ݼ��Ĵ���*/
            if (vkc == 112) /*�������F1��*/
            {
                if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,4);
                        break;
                    case 70:  /*Alt+F*/
                        PopMenu(1);
                        break;
                    case 77: /*Alt+M*/
                        PopMenu(2);
                        break;
                    case 81: /*Alt+Q*/
                        PopMenu(3);
                        break;
                    case 83: /*Alt+S*/
                        PopMenu(4);
                        break;
                    case 72: /*Alt+H*/
                        PopMenu(5);
                        break;
                }
            }
            else if (asc == 0) /*�������Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40:
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)
                            {
                                num = ga_sub_menu_count[gi_sel_menu-1];
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num--;
                            }
                            TagSubMenu(num);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])
                            {
                                num = 1;
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num++;
                            }
                            TagSubMenu(num);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/
                            PopMenu(1);
                            break;
                        case 77: /*m��M*/
                            PopMenu(2);
                            break;
                        case 81: /*q��Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            PopMenu(4);
                            break;
                        case 72: /*h��H*/
                            PopMenu(5);
                            break;
                        case 13: /*�س�*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
}

void PopPrompt(int num)
{

}

int PopInputMenu(char *plabel_name[], int label_num, char *ppcondition[], int hot_area_num) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1, n = label_num;
    int i, maxlen, str_len;
    for (i=0,maxlen=0; i<n; i++)
    {
        str_len = strlen(plabel_name[i]);
        if (maxlen < str_len)
        {
            maxlen = str_len;
        }
    }

    /**************���������ھ���*********************/
    pos.X = maxlen + 6;
    pos.Y = n;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + 27+6;
    rcPop.Top = (SCR_ROW - pos.Y) / 2-10;
    rcPop.Bottom = rcPop.Top + pos.Y + n;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN ;  /*��������������׺���*/
    labels.num = n;
    labels.ppLabel = plabel_name;
    COORD aLoc[n];


    /******���ñ�ǩ�������λ��*****/
    for(i = 0; i < n-1; i++) {
        if (i == 0) {
            aLoc[i].X = rcPop.Left + 2;
            aLoc[i].Y = rcPop.Top + 1;
        }
        else {
            aLoc[i].X = rcPop.Left + 2;
            aLoc[i].Y = aLoc[i-1].Y + 2;
        }
    }
    aLoc[n-1].X = rcPop.Left + (pos.X - strlen(plabel_name[n-1]))/2;
    aLoc[n-1].Y = aLoc[n-2].Y + 2;
    labels.pLoc = aLoc;

    /****����������Ϣ****/
    areas.num = hot_area_num;
    SMALL_RECT aArea[hot_area_num];
    char aSort[hot_area_num];
    for (i = 0; i < hot_area_num-2; i++) {
        aSort[i] = 1;
    }
    aSort[hot_area_num-2] = 0;
    aSort[hot_area_num-1] = 0;
    char aTag[hot_area_num];
    for (i = 0; i < hot_area_num; i++) {
        aTag[i] = i+1;
    }
    for (i=0; i<hot_area_num-2; i++)
    {
        aArea[i].Left = aLoc[i+1].X + strlen(plabel_name[i+1]);
        aArea[i].Top = aLoc[i+1].Y ;
        aArea[i].Right = aLoc[i+1].X + 27;
        aArea[i].Bottom = aLoc[i+1].Y;
    }
    aArea[hot_area_num-2].Left = aLoc[hot_area_num-1].X ;
    aArea[hot_area_num-2].Top = aLoc[hot_area_num-1].Y ;
    aArea[hot_area_num-2].Right = aLoc[hot_area_num-1].X + 3;
    aArea[hot_area_num-2].Bottom = aLoc[hot_area_num-1].Y;
    aArea[hot_area_num-1].Left = aLoc[hot_area_num-1].X + 8;
    aArea[hot_area_num-1].Top = aLoc[hot_area_num-1].Y ;
    aArea[hot_area_num-1].Right = aLoc[hot_area_num-1].X + 11;
    aArea[hot_area_num-1].Bottom = aLoc[hot_area_num-1].Y;

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);
    pos.X = rcPop.Left + 2;
    pos.Y = aLoc[hot_area_num-1].Y+1;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    if (DealInput2(&areas, &iHot, ppcondition) != 13)
    {
        return -1;
    }
    else {
        return iHot;
    }
}

int PopChoiceMenu(char *plabel_name[], int choice_num) {
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1, n = choice_num;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++)
    {
        str_len = strlen(plabel_name[i]);
        if (maxlen < str_len)
        {
            maxlen = str_len;
        }
    }

    /**************���������ھ���*********************/
    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2-8;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN ;  /*��������������׺���*/
    labels.num = n;
    labels.ppLabel = plabel_name;
    COORD aLoc[n];


    /******���ñ�ǩ�������λ��*****/
    for (i=0; i<n; i++)
    {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;

    }
    str_len = strlen(plabel_name[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;

    labels.pLoc = aLoc;

    areas.num = n;
    SMALL_RECT aArea[n];
    char aSort[n];
    char aTag[n];
    for (i=0; i<n; i++)
    {
        aArea[i].Left = aLoc[i].X;
        aArea[i].Top = aLoc[i].Y ;
        aArea[i].Right = aLoc[i].X + strlen(plabel_name[i]) - 1;
        aArea[i].Bottom = aLoc[i].Y;
        aSort[i] = 0;
        aTag[i] = i + 1;
    }

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);
    if (DealInput2(&areas, &iHot, NULL) != 13)
    {
        return -1;
    }
    return iHot;
}
/**
 * ��������: PopMenu
 * ��������: ����ָ�����˵����Ӧ���Ӳ˵�.
 * �������: num ָ�������˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;

    if (num != gi_sel_menu)       /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (gp_top_layer->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
    {
        return;
    }

    gi_sel_menu = num;    /*��ѡ�����˵�����Ϊָ�������˵���*/
    TagMainMenu(gi_sel_menu); /*��ѡ�е����˵����������*/
    LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/

    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD aLoc[labels.num];/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
    for (i=0; i<labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*����������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT aArea[areas.num];                    /*������������������λ��*/
    char aSort[areas.num];                      /*��������������������Ӧ���*/
    char aTag[areas.num];                         /*����������ÿ�������ı��*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*������λ*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1;           /*������˳����*/
    }
    areas.pArea = aArea;/*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;/*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;   /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*���������ڻ��߿�*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*����Ϊ0������Ϊ�մ�*/
        {   /*���Ȼ�����*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*ȡ���������ַ���Ԫ����������*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }

    }
    /*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*�����մ�*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/**
 * ��������: PopUp
 * ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
 * �������: pRc ��������λ�����ݴ�ŵĵ�ַ
 *           att �������������ַ�����
 *           pLabel ���������б�ǩ����Ϣ��ŵĵ�ַ
             pHotArea ����������������Ϣ��ŵĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {
        /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)
                                         | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

/**
 * ��������: PopOff
 * ��������: �رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;

    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*�ָ�������������ԭ���*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**
 * ��������: DrawBox
 * ��������: ��ָ�����򻭱߿�.
 * �������: pRc �������λ����Ϣ�ĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*�����õ��ַ�*/
    COORD pos = {pRc->Left, pRc->Top};  /*��λ����������Ͻ�*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*��ѭ�����ϱ߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*��ѭ�����߿�����ߺ��ұ���*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*���±߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    return;
}

/**
 * ��������: TagSubMenu
 * ��������: ��ָ���Ӳ˵�������ѡ�б��.
 * �������: num ѡ�е��Ӳ˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    LocSubMenu(gi_sel_menu, &rcPop);  /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
        return;
    }

    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*���ƶ��Ӳ˵�������ѡ�б��*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/**
 * ��������: LocSubMenu
 * ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
 * �������: num ѡ�е����˵����
 * �������: rc �������λ����Ϣ�ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }
    rc->Right += rc->Left + 3;  /*����������ұ߽�*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*���������±ߵ��к�*/
    if (rc->Right >= SCR_COL)  /*�ұ߽�Խ��Ĵ���*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}
/**
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: pHotArea
 *           piHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �������: piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �� �� ֵ:
 *
 * ����˵��:
 */
int DealInput2(HOT_AREA *pHotArea, int *piHot, char **ppcondition)
{
    INPUT_RECORD inRec[2];
    DWORD res;
    COORD pos = {0, 0};
    int num = *piHot, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    int i, j , TxtHotAreaNum = 0;

    for (i=0; i<pHotArea->num; i++) /*�����ı�����������*/
    {
        if(pHotArea->pSort[i] == 1)
        {
            TxtHotAreaNum++;
        }
    }
    for (i=0; i<TxtHotAreaNum; i++)
    {
        ppcondition[i] = (char*)malloc(30*sizeof(char));
        ppcondition[i][0] = '\0';
    }

    SetHotPoint(pHotArea, *piHot);

    while (TRUE)
    {
        /*ѭ��*/
        ReadConsoleInput(gh_std_in, &inRec, 2, &res);
        if ((inRec[0].EventType == MOUSE_EVENT) &&
            (inRec[0].Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec[0].Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;   /*ȡ���������ڵĲ���*/
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;  /*ȡ���ַ���Ԫ���������*/
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;  /*ȡ����������*/

            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                num = *piHot;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)  /*ѡ�е���ȡ����ť*/
                {
                    iRet = 13;
                    break;
                }
                else
                {
                    j = 0;
                    for (i=0; i<num; i++)
                    {
                        if(pHotArea->pSort[i] == 1)
                        {
                            j++;
                        }
                    }
                    /**���ù��λ�ã�ʹ��ʼ�ճ������������ַ����ĺ���**/
                    COORD CursorPosition = {pHotArea->pArea[num-1].Left+strlen(ppcondition[j-1]),
                                            pHotArea->pArea[num-1].Top
                    };
                    SetConsoleCursorPosition(gh_std_out, CursorPosition);
                }
            }
        }
        else if (inRec[0].EventType == KEY_EVENT && inRec[0].Event.KeyEvent.bKeyDown)
        {
            vkc = inRec[0].Event.KeyEvent.wVirtualKeyCode;
            asc = inRec[0].Event.KeyEvent.uChar.AsciiChar;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {
                    /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37:
                        arrow = 1;
                        break;
                    case 38:
                        arrow = 2;
                        break;
                    case 39:
                        arrow = 3;
                        break;
                    case 40:
                        arrow = 4;
                        break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if (num == 0)
                        {
                            num = pHotArea->num;
                        }
                        if (num == pHotArea->num + 1)
                        {
                            num = 1;
                        }
                        if (((arrow % 2) && (pHotArea->pArea[num-1].Top
                                             == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                                                                                    && (pHotArea->pArea[num-1].Top
                                                                                        != pHotArea->pArea[*piHot-1].Top)) || num == *piHot)
                        {
                            /*ͬ�е��������Ҽ��л�����ͬ�е����¼��л���������ͬһ������Ҳִ��break*/
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                        if (pHotArea->pSort[num-1] == 1)
                        {
                            j = 0;
                            for (i=0; i<num; i++)
                            {
                                if(pHotArea->pSort[i] == 1)
                                {
                                    j++;
                                }
                            }
                            /**���ù��λ�ã�ʹ��ʼ�ճ������������ַ����ĺ���**/
                            COORD CursorPosition = {pHotArea->pArea[num-1].Left+strlen(ppcondition[j-1]),
                                                    pHotArea->pArea[num-1].Top
                            };
                            SetConsoleCursorPosition(gh_std_out, CursorPosition);
                        }
                    }
                }
            }
            else if (vkc == 8)
            {
                /*�˸��*/
                if (pHotArea->pSort[num-1] == 1)
                {
                    COORD CursorPosition;
                    j=0;
                    for (i=0; i<num; i++)
                    {
                        if(pHotArea->pSort[i] == 1)
                        {
                            j++;
                        }
                    }
                    if (*(ppcondition[j-1]+strlen(ppcondition[j-1])-1) > 31
                        && *(ppcondition[j-1]+strlen(ppcondition[j-1])-1) < 127)/*����Ƿ������ַ�*/
                    {
                        *(ppcondition[j-1]+strlen(ppcondition[j-1])-1) = '\0';
                        CursorPosition.X = pHotArea->pArea[num-1].Left+strlen(ppcondition[j-1]);
                        CursorPosition.Y = pHotArea->pArea[num-1].Top;
                        SetConsoleCursorPosition(gh_std_out, CursorPosition);
                        FillConsoleOutputCharacter(gh_std_out, '\0', 1, CursorPosition, &ul);   /*ɾ����λ���ɿ��ַ����*/
                    }
                    else/*�������ַ������ĺ��֣�ɾ���������ַ�����һ������*/
                    {
                        *(ppcondition[j-1]+strlen(ppcondition[j-1])-2) = '\0';
                        CursorPosition.X = pHotArea->pArea[num-1].Left+strlen(ppcondition[j-1]);
                        CursorPosition.Y = pHotArea->pArea[num-1].Top;
                        SetConsoleCursorPosition(gh_std_out, CursorPosition);
                        FillConsoleOutputCharacter(gh_std_out, '\0', 2, CursorPosition, &ul);/*ɾ����λ���ɿ��ַ����*/
                    }
                }
            }
            else if (vkc == 9)
            {
                /*tab����ʾ�л�����*/

                num++;
                if (num == pHotArea->num + 1)
                {
                    num = 1;
                }
                *piHot = num;
                SetHotPoint(pHotArea, *piHot);
            }
            else if (vkc == 27)
            {
                /*ESC��*/

                iRet = 27;
                /*���ع��*/
                CONSOLE_CURSOR_INFO console_cursor_info;
                console_cursor_info.dwSize = 20;
                console_cursor_info.bVisible = FALSE;
                SetConsoleCursorInfo(gh_std_out, &console_cursor_info);
                break;
            }

            else if (vkc == 13)
            {
                /*�س�����ʾ���µ�ǰ��ť*/
                if (pHotArea->pSort[num-1] == 0) /*�����ǰ��������ǰ���������*/
                {
                    iRet = 13;
                    break;
                }
                else
                {
                    num++;    /*������ı������������л�����һ������*/
                    *piHot = num;
                    SetHotPoint(pHotArea, *piHot);
                    if (pHotArea->pSort[num-1] == 1) /*�����ʱ���ı�������*/
                    {
                        j = 0;
                        for (i=0; i<num; i++)
                        {
                            if(pHotArea->pSort[i] == 1)
                            {
                                j++;
                            }
                        }
                        /**���ù��λ�ã�ʹ��ʼ�ճ������������ַ����ĺ���**/
                        COORD CursorPosition = {pHotArea->pArea[num-1].Left+strlen(ppcondition[j-1]),
                                                pHotArea->pArea[num-1].Top
                        };
                        SetConsoleCursorPosition(gh_std_out, CursorPosition);
                    }
                }
            }
            else  /*������ͨ��*/
            {
                if (pHotArea->pSort[num-1] == 1)
                {
                    char string[10];
                    j=0;
                    for (i=0; i<num; i++)
                    {
                        if(pHotArea->pSort[i] == 1)
                        {
                            j++;
                        }
                    }

                    /*������ַ������ô��ڸ������ĳ���*/
                    if (strlen(ppcondition[j-1])<pHotArea->pArea[num-1].Right - pHotArea->pArea[num-1].Left)
                    {
                        for (i=0; i<res; i++)
                        {
                            string[i] = inRec[i].Event.KeyEvent.uChar.AsciiChar;
                        }
                        string[i] = '\0';
                        strcat(ppcondition[j-1],string);
                        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
                        DWORD NumOfAttsWritten;
                        WORD att_new = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
                        GetConsoleScreenBufferInfo(gh_std_out, &console_screen_buffer_info);
                        printf("%s", string);
                        FillConsoleOutputAttribute(gh_std_out, att_new, strlen(string), console_screen_buffer_info.dwCursorPosition, &NumOfAttsWritten);
                    }
                }
            }
        }
    }
    return iRet;
}
/**
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: pHotArea
 *           piHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �������: piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �� �� ֵ:
 *
 * ����˵��:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*ѭ��*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&
            (inRec.Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;

            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {
                    iRet = 13;
                    break;
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num-1].Top
                                             == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                                                                                    && (pHotArea->pArea[num-1].Top
                                                                                        != pHotArea->pArea[*piHot-1].Top)))
                        {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {  /*ESC��*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {  /*�س�����ո��ʾ���µ�ǰ��ť*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2, att3;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*��ѡ���������ı����Ժڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN ;  /*��׺���*/
    att3 = BACKGROUND_BLUE | BACKGROUND_GREEN |BACKGROUND_RED ;  /*�׵׺���*/
    for (i=0; i<pHotArea->num; i++)
    {
        /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {
            /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
        else
        {
            /*�������ı���*/
            FillConsoleOutputAttribute(gh_std_out, att3, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {
        /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = FALSE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {
        /*�������������ı�����*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}

/**
 * ��������: ExeFunction
 * ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
 * �������: m ���˵����
 *           s �Ӳ˵����
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE �� FALSE
 *
 * ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;

    /*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
    pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;

    pFunction[4] = MaintainCityInfo;
    pFunction[5] = MaintainScenicAreaInfo;
    pFunction[6] = MaintainAttractionInfo;
    pFunction[7] = QueryCityInfo;
    pFunction[8] = QueryScenicAreaInfo;
    pFunction[9] = QueryAttractionInfo;
    pFunction[10] = RandomView;
    pFunction[11] = StatusCityInfo;
    pFunction[12] = HelpTopic;
    pFunction[13] = NULL;
    pFunction[14] = AboutDorm;

    for (i=1,loc=0; i<m; i++)  /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    {
        loc += ga_sub_menu_count[i-1];
    }
    loc += s - 1;

    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*�ú���ָ�������ָ��Ĺ��ܺ���*/
    }

    return bRet;
}

BOOL SaveData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݱ���",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL BackupData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݱ���",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL RestoreData(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݻָ�",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"ȷ���˳�ϵͳ��", "ȷ��    ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 5, rcPop.Top + 5,
                                  rcPop.Left + 8, rcPop.Top + 5},
                          {rcPop.Left + 13, rcPop.Top + 5,
                                  rcPop.Left + 16, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();

    return bRet;
}
BOOL MaintainCityInfo(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"¼�������Ϣ",
                           "�޸ĳ�����Ϣ",
                           "ɾ��������Ϣ",
                           "ȡ��"
    };

    int iHot = PopChoiceMenu(plabel_name, 4);

    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1){
            PopOff();
            InsertCityNodeSubMenu();

        }
        else if (iHot == 2) {
            PopOff();
            ModifyCityNodeSubMenu();
        }
        else if (iHot == 3) {
            PopOff();
            DeleteCityNodeSubMenu();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL InsertCityNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"�������¼��ĳ�����Ϣ",
                           "��  ��ID",
                           "��������",
                           "�ල�绰",
                           "��ѯ�绰",
                           "ȷ��    ȡ��"
    };
    int n = 6;
    int inputNum = 6;
    char *ppcondition[inputNum];
    int iHot = PopInputMenu(plabel_name, n, ppcondition, inputNum );
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 5) {
            PopOff();
        }
        else if (iHot == 6){
            PopOff();
        }
    }
    return bRet;
}

BOOL ModifyCityNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݳ��������޸�",
                           "���ݳ�  ��ID�޸�",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL DeleteCityNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݳ�������ɾ��",
                           "���ݳ�  ��IDɾ��",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL MaintainScenicAreaInfo(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"¼�뾰����Ϣ",
                           "�޸ľ�����Ϣ",
                           "ɾ��������Ϣ",
                           "ȡ��"
    };

    int iHot = PopChoiceMenu(plabel_name, 4);

    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1){
            PopOff();
            InsertScenicAreaNodeSubMenu();

        }
        else if (iHot == 2) {
            PopOff();
            ModifyScenicAreaNodeSubMenu();
        }
        else if (iHot == 3) {
            PopOff();
            DeleteScenicAreaNodeSubMenu();
        }
        else {
            PopOff();
        }
    }
    return bRet;

}

BOOL InsertScenicAreaNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"�������¼��ľ�����Ϣ",
                           "��������",
                           "��  ��ID",
                           "��������",
                           "��������",
                           "������ַ",
                           "��Ʊ�۸�",
                           "����ʱ��",
                           "��ѯ�绰",
                           "ȷ��    ȡ��"
    };
    int n = 10;
    int inputNum = 10;
    char *ppcondition[inputNum];
    int iHot = PopInputMenu(plabel_name, n, ppcondition, inputNum );
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 9) {
            PopOff();
        }
        else if (iHot == 10){
            PopOff();
        }
    }
    return bRet;
}

BOOL ModifyScenicAreaNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ��������޸�",
                           "���ݾ�  ��ID�޸�",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL DeleteScenicAreaNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ�������ɾ��",
                           "���ݾ�  ��IDɾ��",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL MaintainAttractionInfo(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"¼�뾰����Ϣ",
                           "�޸ľ�����Ϣ",
                           "ɾ��������Ϣ",
                           "ȡ��"
    };

    int iHot = PopChoiceMenu(plabel_name, 4);

    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1){
            PopOff();
            InsertAttractionNodeSubMenu();

        }
        else if (iHot == 2) {
            PopOff();
            ModifyAttractionNodeSubMenu();
        }
        else if (iHot == 3) {
            PopOff();
            DeleteAttractionNodeSubMenu();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL InsertAttractionNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"�������¼��ľ�����Ϣ",
                           "��������",
                           "��  ��ID",
                           "��������",
                           "��������",
                           "����λ��",
                           "���ʱ��",
                           "�����ص�",
                           "ȷ��    ȡ��"
    };
    int n = 9;
    int inputNum = 9;
    char *ppcondition[inputNum];
    int iHot = PopInputMenu(plabel_name, n, ppcondition, inputNum );
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 8) {
            PopOff();
        }
        else if (iHot == 9){
            PopOff();
        }
    }
    return bRet;
}

BOOL ModifyAttractionNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ��������޸�",
                           "���ݾ�  ��ID�޸�",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL DeleteAttractionNodeSubMenu(void) {
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ�������ɾ��",
                           "���ݾ�  ��IDɾ��",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL QueryCityInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݳ������Ʋ�ѯ",
                           "���ݳ�  ��ID��ѯ",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL QueryScenicAreaInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ������Ʋ�ѯ",
                           "���ݾ�  ��ID��ѯ",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL RandomView(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"��㿴������",
                           "��㿴������",
                           "��㿴������",
                           "ȡ��"
    };
    int n = 4;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL StatusCityInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"��㿴������",
                           "��㿴������",
                           "��㿴������",
                           "ȡ��"
    };
    int n = 4;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL QueryAttractionInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���ݾ��������޸�",
                           "���ݾ�  ��ID�޸�",
                           "ȡ��"
    };
    int n = 3;
    int iHot = PopChoiceMenu(plabel_name, n);
    if (iHot == -1) {
        PopOff();
    }
    else {
        if (iHot == 1) {
            PopOff();
        }
        else if (iHot == 2){
            PopOff();
        }
        else if (iHot == 3) {
            PopOff();
        }
        else {
            PopOff();
        }
    }
    return bRet;
}

BOOL MaintainCityInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵��������Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL MaintainScenicAreaInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵��������Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL MaintainAttractionInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵��������Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL QueryCityInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵������¥��Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL QueryScenicAreaInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵������������Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL QueryAttractionInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵��������Ϣ",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL RandomView(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ͳ��",
                           "�Ӳ˵����㿴��",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL StatusCityInfo(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ͳ��",
                           "�Ӳ˵�����о���ͳ��",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL HelpTopic(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������",
                           "�Ӳ˵����������",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL AboutThis(void)
{
    BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������",
                           "�Ӳ˵������...",
                           "ȷ��"
    };

    ShowModule(plabel_name, 3);

    return bRet;
}

/**
 * ��������: InsertChargeNode
 * ��������: ��ʮ�������в���һ���ɷ���Ϣ���.
 * �������: hd ����ͷָ��
 *           pSPOT_NODE ָ����Ҫ�������ָ��
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE��ʾ����ɹ�, FALSE��ʾ����ʧ��
 *
 * ����˵��:
 */
BOOL InsertChargeNode(CITY_NODE *hd, SPOT_NODE *pSPOT_NODE)
{

    return TRUE;
}

/**
 * ��������: DelRegionNode
 * ��������: ��ʮ��������ɾ��ָ���ľ������.
 * �������: hd ����ͷָ��
 *           region_id �������
 *           date �ɷ�����
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE��ʾɾ���ɹ�, FALSE��ʾɾ��ʧ��
 *
 * ����˵��: ����ѧ�źͽɷ����ڿ���ȷ��Ψһ�Ľɷ���Ϣ
 */
BOOL DelChargeNode(CITY_NODE *hd, char *region_id, char *date)
{

    return TRUE;
}

/**
 * ��������: ModifChargeNode
 * ��������: ��ָ���Ľɷ���Ϣ������ݽ����޸�.
 * �������: hd ����ͷָ��
 *           region_id �ɷѾ���ѧ��
 *           date �ɷ�����
 *           pSPOT_NODE ָ�����޸����ݽ���ָ��
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE��ʾ�޸ĳɹ�, FALSE��ʾ�޸�ʧ��
 *
 * ����˵��:
 */
BOOL ModifChargeNode(CITY_NODE *hd, char *region_id, char *date, SPOT_NODE *pSPOT_NODE)
{

    return FALSE;
}

/**
 * ��������: SeekStuNode
 * ��������: ��ѧ�Ų��Ҿ���������Ϣ���.
 * �������: hd ����ͷָ��
 *           region_id ����ѧ��
 * �������: ��
 * �� �� ֵ: ����ʱ���ؽ��ĵ�ַ, ���򷵻�NULL
 *
 * ����˵��:
 */
REGION_NODE *SeekStuNode(CITY_NODE *hd, char *region_id)
{

    return NULL;
}

/**
 * ��������: SeekChargeNode
 * ��������: ��ѧ�źͽɷ����ڲ��ҽɷ���Ϣ���.
 * �������: hd ����ͷָ��
 *           region_id ����ѧ��
 *           date �ɷ�����
 * �������: ��
 * �� �� ֵ: ����ʱ���ؽ��ĵ�ַ, ���򷵻�NULL
 *
 * ����˵��:
 */
SPOT_NODE *SeekChargeNode(CITY_NODE *hd, char *region_id, char *date)
{

    return NULL;
}

/**
 * ��������: SeekStuNodeM
 * ��������: ������������ϲ�ѯ�������������о�����Ϣ���.
 * �������: hd ����ͷָ��
 *           cond_num ��������ĸ���
 *           ... ��ʾ��ѯ�������ַ���
 * �������: ��
 * �� �� ֵ: ���������������Ľ�㸴�Ƶ�����������ؽ�������ͷ����ַ
 *
 * ����˵��:
 */
REGION_NODE *SeekStuNodeM (CITY_NODE *hd, int cond_num, ...)
{

    return NULL;
}

/**
 * ��������: JudgeStuNodeItem
 * ��������: �жϾ�����Ϣ����Ƿ������������.
 * �������: pREGION_NODE ������Ϣ���ָ��
 *           pcondition ������ʾ�������ַ���
 * �������: ��
 * �� �� ֵ: ��������ʱ, ����TRUE; ���򷵻�FALSE
 *
 * ����˵��:
 */
BOOL JudgeStuNodeItem(REGION_NODE *pREGION_NODE, char *pcondition)
{

    return TRUE;
}

/**
 * ��������: MatchString
 * ��������: �Ը����ַ�������������ƥ��.
 * �������: string_item �����ַ���
 *           pcond ����ƥ����������ڵ������ַ���
 * �������: ��
 * �� �� ֵ: ƥ��ɹ�ʱ, ����TRUE; ���򷵻�FALSE
 *
 * ����˵��:
 */
BOOL MatchString(char *string_item, char *pcond)
{

    return TRUE;
}

/**
 * ��������: MatchChar
 * ��������: �Ը����ַ�����������ƥ��.
 * �������: char_item �����ַ�
 *           pcond ����ƥ����������ڵ������ַ���
 * �������: ��
 * �� �� ֵ: ƥ��ɹ�ʱ, ����TRUE; ���򷵻�FALSE
 *
 * ����˵��:
 */
BOOL MatchChar(char char_item, char *pcond)
{

    return TRUE;
}

/**
 * ��������: StatUnchargeInfo
 * ��������: ͳ��δ�ɷ���Ϣ.
 * �������: hd ����ͷ���ָ��
 * �������: ��
 * �� �� ֵ: ����ͳ�ƽ����ͷ����ַ
 *
 * ����˵��:
 */
UNCHARGE_NODE *StatUnchargeInfo(CITY_NODE *hd)
{

    return NULL;
}

/**
 * ��������: SortUnchargeInfo
 * ��������: ��Ƿ����Ϣ����������.
 * �������: uncharge_hd Ƿ����Ϣ��ͷ���ָ��
 * �������: uncharge_hd ������ͬʱͨ��ͷ���ָ�뷵��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void SortUnchargeInfo(UNCHARGE_NODE *uncharge_hd)
{

    return;
}

/**
 * ��������: SaveSysData
 * ��������: ����ϵͳ�����������������.
 * �������: hd ����ͷ���ָ��
 * �������:
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL SaveSysData(CITY_NODE *hd)
{

    return TRUE;
}

/**
 * ��������: BackupSysData
 * ��������: ��ϵͳ����������������ݱ��ݵ�һ�������ļ�.
 * �������: hd ����ͷ���ָ��
 *           filename �����ļ���
 * �������:
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL BackupSysData(CITY_NODE *hd, char *filename)
{

    return TRUE;
}

/**
 * ��������: RestoreSysData
 * ��������: ��ָ�������ļ��лָ�ϵͳ�����������������.
 * �������: phead ����ͷ���ָ��ĵ�ַ
 *           filename ��ű������ݵ������ļ���
 * �������:
 * �� �� ֵ: BOOL����, ����ΪTRUE
 *
 * ����˵��:
 */
BOOL RestoreSysData(CITY_NODE **phead, char *filename)
{

    return TRUE;
}

BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];

    for (i=0; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;

    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;

    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,
                                  aLoc[n-1].X + 3, aLoc[n-1].Y}};

    char aSort[] = {0};
    char aTag[] = {1};

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    DealInput(&areas, &iHot);
    PopOff();

    return bRet;

}


BOOL add_city(CITY_NODE **head, CITY_NODE *pcity_node) {
    CITY_NODE *test;
    pcity_node->rnext = NULL;
    pcity_node->next = NULL;

    test = *head;
    while (test != NULL)
    {
        if (strcmp(test->city_id, pcity_node->city_id) == 0 ||
            strcmp(test->name, pcity_node->name) == 0)
        {
            return FALSE;
        }
        test = test->next;
    }
    if (*head == NULL) {
        *head = pcity_node;
        (*head)->next = NULL;
        (*head)->rnext = NULL;
    }
    else {
        pcity_node->next = (*head)->next;
        (*head)->next = pcity_node;
    }
    return TRUE;
}