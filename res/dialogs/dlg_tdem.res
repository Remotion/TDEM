// C4D-DialogResource
DIALOG DLG_TDEM
{
  NAME DLG_TDEM;
  
  TAB IDC_TDM_TBGR1
  {
    NAME IDC_TDM_TBGR1; ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
    SELECTION_TABS; 
    
    GROUP IDC_TDM_GRL1
    {
      NAME IDC_TDM_GRL1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      GROUP IDC_TDM_GRL11
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 4, 4;
        
        STATICTEXT IDC_TDM_STATIC2 { NAME IDC_TDM_STATIC2; CENTER_V; ALIGN_LEFT; SIZE 120, 0; }
        EDITTEXT IDC_TDM_EDN3
        {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_TDM_N3
        {CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
      }
      GROUP IDC_TDM_GRL12
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 4, 4;
        
        STATICTEXT IDC_TDM_STATIC3 { NAME IDC_TDM_STATIC3; CENTER_V; FIT_H; SIZE 130, 0; }
        EDITTEXT IDC_TDM_EDN4
        {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_TDM_N4
        {CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
        STATICTEXT IDC_TDM_STATIC4 { NAME IDC_TDM_STATIC4; CENTER_V; ALIGN_LEFT; SIZE 130, 0; }
        EDITTEXT IDC_TDM_SEL4
        {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_TDM_SEL_N4
        {CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
        STATICTEXT IDC_TDM_STATIC41 { NAME IDC_TDM_STATIC41; CENTER_V; ALIGN_LEFT; SIZE 130, 0; }
        EDITTEXT IDC_TDM_VEX4
        {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_TDM_VEX_N4
        {CENTER_V; CENTER_H; SIZE 50, 0; }
      }
      GROUP IDC_TDM_GRL13
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 2;
        SPACE 6, 6;
        
        STATICTEXT IDC_TDM_STATIC5 { NAME IDC_TDM_STATIC5; CENTER_V; ALIGN_LEFT; SIZE 180, 0; }
        EDITNUMBERARROWS IDC_TDM_ED1
        {CENTER_V; ALIGN_LEFT; SIZE 100, 0; }
        STATICTEXT IDC_TDM_DST1 { NAME IDC_TDM_DST1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
        COMBOBOX IDC_TDM_DIR1
        {
          CENTER_V; FIT_H; SCALE_H; SIZE 150, 0; 
          CHILDS
          {
            0, IDC_TDM_DIR11; 
            1, IDC_TDM_DIR12; 
            2, IDC_TDM_DIR13; 
          }
        }
        CHECKBOX IDC_TDM_CHECK1 { NAME IDC_TDM_CHECK1; CENTER_V; FIT_H; SCALE_H; SIZE 180, 0;  }
      }
      GROUP IDC_TDM_GRL14
      {
        ALIGN_TOP; FIT_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        ROWS 2;
        SPACE 2, 2;
        
        STATICTEXT IDC_TDM_STATIC5_X { NAME IDC_TDM_STATIC5_X; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_ED2_X
        {CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_TDM_STATIC5_Y { NAME IDC_TDM_STATIC5_Y; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_ED2_Y
        {CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_TDM_STATIC5_Z { NAME IDC_TDM_STATIC5_Z; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_ED2_Z
        {CENTER_V; CENTER_H; SIZE 100, 0; }
      }
      GROUP IDC_TDM_GRL15
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 6, 6;
        
        STATICTEXT IDC_TDM_STATIC7 { NAME IDC_TDM_STATIC7; CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
        EDITNUMBERARROWS IDC_TDM_RND5
        {CENTER_V; FIT_H; SCALE_H; SIZE 80, 0; }
        BUTTON IDC_TDM_BT1 { NAME IDC_TDM_BT1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
      }
    }
    GROUP IDC_TDM_GRL2
    {
      NAME IDC_TDM_GRL2; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      GROUP IDC_TDM_GRL21
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 1;
        SPACE 4, 4;
        
        GROUP IDC_TDM_GRL211
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 4;
          SPACE 4, 4;
          
          CHECKBOX IDC_TDM_CHECK3 { NAME IDC_TDM_CHECK3; CENTER_V; ALIGN_LEFT; SIZE 130, 0;  }
          EDITTEXT IDC_TDM_EDN5
          {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
          EDITNUMBERARROWS IDC_TDM_N5
          {CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
        }
        GROUP IDC_TDM_GRL212
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 4;
          SPACE 4, 4;
          
          STATICTEXT IDC_TDM_STATIC8 { NAME IDC_TDM_STATIC8; CENTER_V; ALIGN_LEFT; SIZE 130, 0; }
          EDITTEXT IDC_TDM_SEL5
          {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
          EDITNUMBERARROWS IDC_TDM_SEL_N5
          {CENTER_V; CENTER_H; SIZE 50, 0; }
        }
        GROUP IDC_TDM_GRL213
        {
          ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 2;
          SPACE 4, 4;
          
          CHECKBOX IDC_TDM_CHD2 { NAME IDC_TDM_CHD2; CENTER_V; ALIGN_LEFT; SIZE 180, 0;  }
          CHECKBOX IDC_TDM_CHR2 { NAME IDC_TDM_CHR2; CENTER_V; ALIGN_LEFT; SIZE 0, 0;  }
          CHECKBOX IDC_TDM_CHS1 { NAME IDC_TDM_CHS1; CENTER_V; ALIGN_LEFT; SIZE 180, 0;  }
          CHECKBOX IDC_TDM_CHS2 { NAME IDC_TDM_CHS2; CENTER_V; ALIGN_LEFT; SIZE 180, 0;  }
          STATICTEXT IDC_TDM_TEXT13 { NAME IDC_TDM_TEXT13; CENTER_V; ALIGN_LEFT; SIZE 180, 0; }
          COMBOBOX IDC_TDM_DD_OM5
          {
            CENTER_V; ALIGN_LEFT; SIZE 93, 0; 
            CHILDS
            {
              0, IDC_TDM_DD02; 
              1, IDC_TDM_DD12; 
              2, IDC_TDM_DD22; 
            }
          }
          STATICTEXT IDC_TDM_TEXT12 { NAME IDC_TDM_TEXT12; CENTER_V; ALIGN_LEFT; SIZE 180, 0; }
          EDITNUMBERARROWS IDC_TDM_NN5
          { CENTER_V; ALIGN_LEFT; SIZE 100, 0; }
        }
      }
      GROUP IDC_TDM_GRL23
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 1;
        SPACE 4, 4;
        
        GROUP IDC_TDM_GRL231
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 4;
          SPACE 4, 4;
          
          CHECKBOX IDC_TDM_PT_CB4 { NAME IDC_TDM_EPT_CB4; CENTER_V; ALIGN_LEFT; SIZE 130, 0;  }
          EDITTEXT IDC_TDM_PT4
          {CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
          EDITNUMBERARROWS IDC_TDM_PT_N4
          {CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
        }
        GROUP IDC_TDM_GRL232
        {
          ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 2;
          SPACE 4, 4;
          
          CHECKBOX IDC_TDM_AT_CB4 { NAME IDC_TDM_AT_CB4; CENTER_V; ALIGN_LEFT; SIZE 0, 0;  }
          CHECKBOX IDC_TDM_AT_FF4 { NAME IDC_TDM_AT_FF4; CENTER_V; ALIGN_LEFT; SIZE 0, 0;  }
          STATICTEXT IDC_TDM_TEXT21 { NAME IDC_TDM_TEXT21; CENTER_V; ALIGN_LEFT; SIZE 180, 0; }
          COMBOBOX IDC_TDM_DD_OM4
          {
            CENTER_V; ALIGN_LEFT; SIZE 93, 0; 
            CHILDS
            {
              0, IDC_TDM_DD0; 
              1, IDC_TDM_DD1; 
              2, IDC_TDM_DD2; 
            }
          }
          STATICTEXT IDC_TDM_TEXT11 { NAME IDC_TDM_TEXT11; CENTER_V; ALIGN_LEFT; SIZE 180, 0; }
          EDITNUMBERARROWS IDC_TDM_NN4
          {CENTER_V; ALIGN_LEFT; SIZE 100, 0; }
        }
      }
    }
    GROUP IDC_TDM_GRL3
    {
      NAME IDC_TDM_GRL3; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      GROUP IDC_TDM_GRL31
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 4, 4;
        
        STATICTEXT IDC_TDM_TEXT31 { NAME IDC_TDM_TEXT31; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
        COMBOBOX IDC_TDM_CHA1
        {
          CENTER_V; ALIGN_LEFT; SIZE 150, 0; 
          CHILDS
          {
            0, CH_COLOR; 
            1, CH_LUMINANCE; 
            2, CH_TRANSPARENCY; 
            3, CH_REFLECTION; 
            4, CH_ENVIRONMENT; 
            6, CH_BUMP; 
            7, CH_ALPHA; 
            9, CH_SPECULARCOLOR; 
            11, CH_DISPLACEMENT; 
            12, CH_DIFFUSION; 
            20, CH_NONE; 
          }
        }
        EDITNUMBERARROWS IDC_TDM_TEX_N1
        { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
      }
    }
    GROUP IDC_TDM_GRL4
    {
      NAME IDC_TDM_GRL4; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      GROUP IDC_TDM_GRL41
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 2;
        SPACE 4, 4;
        
        CHECKBOX IDC_TDM_PPCOLL { NAME IDC_TDM_PPCOLL; CENTER_V; ALIGN_LEFT; SIZE 0, 0;  }
        STATICTEXT IDC_TDM_TEXT34 { CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
        STATICTEXT IDC_TDM_TEXT32 { NAME IDC_TDM_TEXT32; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
        EDITNUMBERARROWS IDC_TDM_PPOVERSAM
        { CENTER_V; ALIGN_LEFT; SIZE 100, 0; }
        STATICTEXT IDC_TDM_TEXT33 { NAME IDC_TDM_TEXT33; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
        EDITNUMBERARROWS IDC_TDM_PPRADIUS
        { CENTER_V; ALIGN_LEFT; SIZE 100, 0; }
      }
      GROUP IDC_TDM_GRL42
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        ROWS 2;
        SPACE 4, 4;
        
        STATICTEXT IDC_TDM_ST41_X { NAME IDC_TDM_ST41_X; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_GRV1_X
        {CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_TDM_ST41_Y { NAME IDC_TDM_ST41_Y; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_GRV1_Y
        {CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_TDM_ST41_Z { NAME IDC_TDM_ST41_Z; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_TDM_GRV1_Z
        {CENTER_V; CENTER_H; SIZE 100, 0; }
      }
    }
  }
  DLGGROUP { OK; CANCEL; HELP; }
}