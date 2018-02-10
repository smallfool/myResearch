#pragma once

///û�����ü������ӵ��Զ�Release�Ľӿ�ָ��
///����
template<typename IPointer>
class TAutoRelease
{
public:
	TAutoRelease(IPointer* p=NULL)
		:m_Interface(p)
	{
	}
	~TAutoRelease()
	{
		if (m_Interface)
		{
			m_Interface->Release();
			m_Interface = NULL;
		}
	}
	void Set(IPointer* p) { m_Interface = p; }
	IPointer* Get() { return m_Interface; }
	const IPointer*& Get() const { return m_Interface; }
private:
	IPointer* m_Interface;
};

