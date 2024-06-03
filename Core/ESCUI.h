#pragma once
#include "PanelUI.h"
class ESCUI :
    public PanelUI
{
    // PanelUI을(를) 통해 상속됨
    void drawForm() override;
    virtual void update() override;
};

