// TalkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015Remote.h"
#include "TalkDlg.h"
#include "afxdialogex.h"

// CTalkDlg �Ի���

IMPLEMENT_DYNAMIC(CTalkDlg, CDialog)

CTalkDlg::CTalkDlg(CWnd* pParent,IOCPServer* IOCPServer, CONTEXT_OBJECT* ContextObject)
	: CDialog(CTalkDlg::IDD, pParent)
{
	m_iocpServer	= IOCPServer;
	m_ContextObject	= ContextObject;
}

CTalkDlg::~CTalkDlg()
{
}

void CTalkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TALK, m_EditTalk);
}


BEGIN_MESSAGE_MAP(CTalkDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TALK, &CTalkDlg::OnBnClickedButtonTalk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTalkDlg ��Ϣ��������


BOOL CTalkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BYTE bToken = COMMAND_NEXT;  
	m_iocpServer->OnClientPreSending(m_ContextObject, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTalkDlg::OnBnClickedButtonTalk()
{
	int iLength = m_EditTalk.GetWindowTextLength();   //EditBox �ϻ�����ݳ���

	if (!iLength)
	{
		return;
	}

	CString strData;
	m_EditTalk.GetWindowText(strData);            //EditBox �ϻ������

	char* szBuffer = new char[iLength + 1];		// 2019.1.5 �����1
	memset(szBuffer,0,sizeof(char)*iLength);

	strcpy(szBuffer,strData.GetBuffer(0));

	m_EditTalk.SetWindowText(NULL);          //EditBox �ϵ��������

	m_iocpServer->OnClientPreSending(m_ContextObject, (LPBYTE)szBuffer, strlen(szBuffer));//���Լ��ڴ��е����ݷ���
	delete [] szBuffer;
}


BOOL CTalkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// ����VK_ESCAPE��VK_DELETE
		if (pMsg->wParam == VK_ESCAPE)
			return true;
		//����ǿɱ༭��Ļس���
		if (pMsg->wParam == VK_RETURN && pMsg->hwnd == m_EditTalk.m_hWnd)
		{
			OnBnClickedButtonTalk(); 

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CTalkDlg::OnClose()
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	m_ContextObject->v1 = 0;
	CancelIo((HANDLE)m_ContextObject->sClientSocket);
	closesocket(m_ContextObject->sClientSocket);
	CDialog::OnClose();
	delete this;
}