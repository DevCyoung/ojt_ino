#pragma once
#include "PanelUI.h"
class ESCUI :
    public PanelUI
{
    // PanelUI��(��) ���� ��ӵ�
    void drawForm() override;
    virtual void update() override;
};

