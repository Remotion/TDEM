// C4D-DialogResource
DIALOG DLG_PTP
{
  NAME DLG_PTP;
  
  STATICTEXT IDC_PTP_ST1 { NAME IDC_PTP_ST1; CENTER_V; CENTER_H; SIZE 0, 0; }
  GROUP 
  {
    CENTER_V; FIT_H; SCALE_H; SIZE 0, 0; 
    BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 2;
    SPACE 4, 4;
    
    STATICTEXT IDC_PTP_STIN1 { NAME IDC_PTP_STIN1; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
    STATICTEXT IDC_PTP_IN1 { NAME IDC_PTP_IN1; CENTER_V; FIT_H; SCALE_H; SIZE 0, 0; }
  }
  GROUP 
  {
    ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
    BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 2;
    SPACE 4, 4;
    
    STATICTEXT IDC_PTP_ST2 { NAME IDC_PTP_ST2; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    COMBOBOX IDC_PTP_SYSTYP
    {
      ALIGN_TOP; FIT_H; SIZE 160, 0; 
      CHILDS
      {
        0, SYSTYP_0; 
        1, SYSTYP_1; 
        2, SYSTYP_2; 
        3, SYSTYP_3; 
		4, SYSTYP_4; 
      }
    }
    STATICTEXT IDC_PTP_ST5 { NAME IDC_PTP_ST5; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    COMBOBOX IDC_PTP_PST1
    {
      ALIGN_TOP; FIT_H; SIZE 160, 0; 
      CHILDS
      {
        0, PST1_0; 
        1, PST1_1; 
      }
    }
    STATICTEXT IDC_PTP_ST3 { NAME IDC_PTP_ST3; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    EDITNUMBERARROWS IDC_PTP_COUNT1
    { CENTER_V; FIT_H; SIZE 50, 0; }
    CHECKBOX IDC_PTP_ANIM { NAME IDC_PTP_ANIM; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0;  }
  }
  GROUP 
  {
    CENTER_V; FIT_H; SCALE_H; SIZE 0, 0; 
    BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 3;
    SPACE 4, 4;
    
    BUTTON IDC_PTP_MS1 { NAME IDC_PTP_MS1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    STATICTEXT IDC_PTP_ST6 { NAME IDC_PTP_ST6; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    EDITNUMBERARROWS IDC_PTP_PPC1
    { CENTER_V; CENTER_H; SIZE 70, 0; }
  }
  GROUP 
  {
    CENTER_V; FIT_H; SCALE_H; SIZE 0, 0; 
    BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 2;
    SPACE 4, 4;
    
    STATICTEXT IDC_PTP_STIN2 { NAME IDC_PTP_STIN2; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
    STATICTEXT IDC_PTP_IN2 { NAME IDC_PTP_IN2; CENTER_V; FIT_H; SCALE_H; SIZE 0, 0; }
  }
  GROUP 
  {
    ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
    BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 3;
    SPACE 4, 4;
    
    STATICTEXT IDC_PTP_ST4 { NAME IDC_PTP_ST4; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
    EDITTEXT IDC_PTP_POITOBJECT
    { CENTER_V; FIT_H; SIZE 160, 0; }
    EDITNUMBERARROWS IDC_PTP_POITOBJECT_NUM
    { CENTER_V; CENTER_H; SIZE 50, 0; }
  }
  DLGGROUP { OK; CANCEL; HELP; }
}