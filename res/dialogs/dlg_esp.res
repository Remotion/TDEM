// C4D-DialogResource
DIALOG DLG_ESP
{
  NAME DLG_ESP;
  
  STATICTEXT IDC_ESP_REM { NAME IDC_ESP_REM; CENTER_V; CENTER_H; SIZE 0, 0; }
  TAB 
  {
    FIT_V; SCALE_V; FIT_H; SCALE_H; SIZE 0, 0; 
    SELECTION_TABS; 
    
    GROUP IDC_ESP_TAB1
    {
      NAME IDC_ESP_TAB1; ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
      BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
      COLUMNS 2;
      SPACE 4, 4;
      
      STATICTEXT IDC_ESP_ST4 { NAME IDC_ESP_ST4; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
      COMBOBOX IDC_ESP_SYST1
      {
        CENTER_V; ALIGN_LEFT; SIZE 150, 0; 
        CHILDS
        {
          0, SYST0; 
          1, SYST1; 
          2, SYST2; 
          3, SYST3; 
        }
      }
      STATICTEXT IDC_ESP_ST1 { NAME IDC_ESP_ST1; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_ELAST
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST2 { NAME IDC_ESP_ST2; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_RESIS
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST3 { NAME IDC_ESP_ST3; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_PPLEN
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST9 { NAME IDC_ESP_ST9; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_MPLEN
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST5 { NAME IDC_ESP_ST5; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_MASSE
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST6 { NAME IDC_ESP_ST6; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_GRAVITI_X
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST7 { NAME IDC_ESP_ST7; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_GRAVITI_Y
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      STATICTEXT IDC_ESP_ST8 { NAME IDC_ESP_ST8; ALIGN_TOP; FIT_H; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_GRAVITI_Z
      { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
      CHECKBOX IDC_ESP_ANIM { NAME IDC_ESP_ANIM; ALIGN_TOP; FIT_H; SIZE 0, 0;  }
    }
    GROUP IDC_ESP_TAB2
    {
      NAME IDC_ESP_TAB2; ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
      BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
      COLUMNS 2;
      SPACE 4, 4;
      
      STATICTEXT IDC_ESP_S2T1 { NAME IDC_ESP_S2T1; ALIGN_TOP; ALIGN_LEFT; SIZE 150, 0; }
      EDITTEXT IDC_ESP_COLL
      { CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
      STATICTEXT IDC_ESP_S2T2 { NAME IDC_ESP_S2T2; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_OVER
      { CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
      STATICTEXT IDC_ESP_S2T3 { NAME IDC_ESP_S2T3; ALIGN_TOP; ALIGN_LEFT; SIZE 150, 0; }
      EDITNUMBERARROWS IDC_ESP_RAD1
      { CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
      STATICTEXT IDC_ESP_S2T4 { NAME IDC_ESP_S2T4; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
      EDITNUMBERARROWS IDC_ESP_PRAD2
      { CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
    }
  }
  DLGGROUP { OK; CANCEL; }
}