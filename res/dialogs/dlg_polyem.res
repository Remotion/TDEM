// C4D-DialogResource
DIALOG DLG_POLYEM
{
  NAME DLG_POLYEM;
  
  TAB IDC_PLE_TBGR1
  {
    NAME IDC_PLE_TBGR1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
    SELECTION_TABS; 
    
    GROUP IDC_PLE_GRL1
    {
      NAME IDC_PLE_GRL1; FIT_V; FIT_H; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 0, 0;
      
      GROUP IDC_PLE_GRL11
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 4, 4;
        
        STATICTEXT IDC_PLE_STATIC2 { NAME IDC_PLE_STATIC2; CENTER_V; ALIGN_LEFT; SIZE 140, 0; }
        EDITTEXT IDC_PLE_EDN3
        { CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_PLE_N3
        { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
      }
      GROUP IDC_PLE_GRL12
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 4, 4;
        
        STATICTEXT IDC_PLE_STATIC3 { NAME IDC_PLE_STATIC3; CENTER_V; FIT_H; SIZE 140, 0; }
        EDITTEXT IDC_PLE_EDN4
        { CENTER_V; ALIGN_LEFT; SIZE 160, 0; }
        EDITNUMBERARROWS IDC_PLE_N4
        { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
      }
      GROUP IDC_PLE_GRL13
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 2;
        SPACE 6, 6;
        
        STATICTEXT IDC_PLE_STATIC5 { NAME IDC_PLE_STATIC5; CENTER_V; ALIGN_LEFT; SIZE 140, 0; }
        EDITNUMBERARROWS IDC_PLE_ED1
        { CENTER_V; ALIGN_LEFT; SIZE 80, 0; }
        STATICTEXT IDC_PLE_DST1 { NAME IDC_PLE_DST1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
        COMBOBOX IDC_PLE_DIR1
        {
          CENTER_V; FIT_H; SCALE_H; SIZE 100, 0; 
          CHILDS
          {
            0, IDC_PLE_DIR11; 
            1, IDC_PLE_DIR12; 
            2, IDC_PLE_DIR13; 
            3, IDC_PLE_DIR14; 
          }
        }
        CHECKBOX IDC_PLE_CHECK1 { NAME IDC_PLE_CHECK1; CENTER_V; FIT_H; SCALE_H; SIZE 180, 0;  }
        CHECKBOX IDC_PLE_VISIBLE { NAME IDC_PLE_VISIBLE; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0;  }
      }
      GROUP IDC_PLE_GRL14
      {
        ALIGN_TOP; FIT_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        ROWS 2;
        SPACE 2, 2;
        
        STATICTEXT IDC_PLE_STATIC5_X { NAME IDC_PLE_STATIC5_X; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_PLE_ED2_X
        { CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_PLE_STATIC5_Y { NAME IDC_PLE_STATIC5_Y; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_PLE_ED2_Y
        { CENTER_V; CENTER_H; SIZE 100, 0; }
        STATICTEXT IDC_PLE_STATIC5_Z { NAME IDC_PLE_STATIC5_Z; CENTER_V; CENTER_H; SIZE 120, 0; }
        EDITNUMBERARROWS IDC_PLE_ED2_Z
        { CENTER_V; CENTER_H; SIZE 100, 0; }
      }
      GROUP IDC_PLE_GRL15
      {
        ALIGN_TOP; FIT_H; SCALE_H; SIZE 0, 0; 
        BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
        COLUMNS 3;
        SPACE 6, 6;
        
        STATICTEXT IDC_PLE_STATIC7 { NAME IDC_PLE_STATIC7; CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
        EDITNUMBERARROWS IDC_PLE_RND5
        { CENTER_V; FIT_H; SCALE_H; SIZE 80, 0; }
        BUTTON IDC_PLE_BT1 { NAME IDC_PLE_BT1; ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; }
      }
    }
    GROUP IDC_PLE_GRL2
    {
      NAME IDC_PLE_GRL2; FIT_V; FIT_H; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      SCROLLGROUP 
      {
        FIT_V; SCALE_V; FIT_H; SCALE_H; SIZE 0, 0; 
        SCROLL_V; 
        
        GROUP 
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_PLE_PARAM0
          {
            ALIGN_TOP; FIT_H; SIZE 0, 0; 
            BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 2;
            SPACE 4, 4;
            
            STATICTEXT IDC_PLE_SPEEDCHAN_TX { NAME IDC_PLE_SPEEDCHAN_TX; CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
            COMBOBOX IDC_PLE_SPEEDCHAN
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, KN_N; 
                1, KN_1; 
                2, KN_2; 
                3, KN_3; 
                4, KN_4; 
                5, KN_5; 
                6, KN_6; 
              }
            }
            STATICTEXT IDC_PLE_SPEEDFIL_TX { NAME IDC_PLE_SPEEDFIL_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
            COMBOBOX IDC_PLE_SPEEDFIL
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, FL_ALL; 
                1, FL_R; 
                2, FL_G; 
                3, FL_B; 
                4, FL_H; 
                5, FL_S; 
                6, FL_V; 
              }
            }
          }
          GROUP IDC_PLE_PARAM1
          {
            ALIGN_TOP; FIT_H; SIZE 0, 0; 
            BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 2;
            SPACE 4, 4;
            
            STATICTEXT IDC_PLE_VISICHAN_TX { NAME IDC_PLE_VISICHAN_TX; CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
            COMBOBOX IDC_PLE_VISICHAN
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, KN_N; 
                1, KN_1; 
                2, KN_2; 
                3, KN_3; 
                4, KN_4; 
                5, KN_5; 
                6, KN_6; 
              }
            }
            STATICTEXT IDC_PLE_VISIFIL_TX { NAME IDC_PLE_VISIFIL_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
            COMBOBOX IDC_PLE_VISIFIL
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, FL_ALL; 
                1, FL_R; 
                2, FL_G; 
                3, FL_B; 
                4, FL_H; 
                5, FL_S; 
                6, FL_V; 
              }
            }
          }
          GROUP IDC_PLE_PARAM2
          {
            ALIGN_TOP; FIT_H; SIZE 0, 0; 
            BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 2;
            SPACE 4, 4;
            
            STATICTEXT IDC_PLE_OFFCHAN_TX { NAME IDC_PLE_OFFCHAN_TX; CENTER_V; ALIGN_LEFT; SIZE 150, 0; }
            COMBOBOX IDC_PLE_OFFCHAN
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, KN_N; 
                1, KN_1; 
                2, KN_2; 
                3, KN_3; 
                4, KN_4; 
                5, KN_5; 
                6, KN_6; 
              }
            }
            STATICTEXT IDC_PLE_OFFFIL_TX { NAME IDC_PLE_OFFFIL_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
            COMBOBOX IDC_PLE_OFFFIL
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 100, 0; 
              CHILDS
              {
                0, FL_ALL; 
                1, FL_R; 
                2, FL_G; 
                3, FL_B; 
                4, FL_H; 
                5, FL_S; 
                6, FL_V; 
              }
            }
            STATICTEXT IDC_PLE_OFFMAX_TX { NAME IDC_PLE_OFFMAX_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
            EDITNUMBERARROWS IDC_PLE_OFFMAX
            { CENTER_V; ALIGN_LEFT; SIZE 80, 0; }
          }
        }
      }
    }
    GROUP IDC_PLE_GRL3
    {
      NAME IDC_PLE_GRL3; FIT_V; FIT_H; SIZE 0, 0; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      SPACE 4, 4;
      
      SCROLLGROUP 
      {
        FIT_V; SCALE_V; FIT_H; SCALE_H; SIZE 0, 0; 
        SCROLL_V; 
        
        GROUP 
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
          BORDERSIZE 1, 1, 1, 1; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_PLE_CHGR1
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL1_TX { NAME IDC_PLE_CHANNEL1_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL1
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE1_TX { NAME IDC_PLE_TEXTURE1_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE1
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT1 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER1 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
          GROUP IDC_PLE_CHGR2
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL2_TX { NAME IDC_PLE_CHANNEL2_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL2
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE2_TX { NAME IDC_PLE_TEXTURE2_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE2
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT2 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER2 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
          GROUP IDC_PLE_CHGR3
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL3_TX { NAME IDC_PLE_CHANNEL3_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL3
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE3_TX { NAME IDC_PLE_TEXTURE3_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE3
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT3 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER3 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
          GROUP IDC_PLE_CHGR4
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL4_TX { NAME IDC_PLE_CHANNEL4_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL4
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE4_TX { NAME IDC_PLE_TEXTURE4_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE4
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT4 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER4 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
          GROUP IDC_PLE_CHGR5
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL5_TX { NAME IDC_PLE_CHANNEL5_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL5
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE5_TX { NAME IDC_PLE_TEXTURE5_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE5
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT5 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER5 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
          GROUP IDC_PLE_CHGR6
          {
            ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 8, 4, 8, 4; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_PLE_CHANNEL6_TX { NAME IDC_PLE_CHANNEL6_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              COMBOBOX IDC_PLE_CHANNEL6
              {
                CENTER_V; ALIGN_LEFT; SIZE 120, 0; 
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
              STATICTEXT IDC_PLE_TEXTURE6_TX { NAME IDC_PLE_TEXTURE6_TX; CENTER_V; ALIGN_LEFT; SIZE 0, 0; }
              EDITNUMBERARROWS IDC_PLE_TEXTURE6
              { CENTER_V; ALIGN_LEFT; SIZE 50, 0; }
              BUTTON IDC_PLE_EDIT6 { NAME IDC_PLE_EDIT; ALIGN_TOP; FIT_H; SIZE 0, 0; }
            }
            GROUP 
            {
              ALIGN_TOP; ALIGN_LEFT; SIZE 0, 0; 
              BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 1, 1, 1, 1; 
              COLUMNS 1;
              SPACE 4, 4;
              
              USERAREA IDC_PLE_USER6 { ALIGN_TOP; FIT_H; SIZE 80, 80; }
            }
          }
        }
      }
    }
  }
  DLGGROUP { OK; CANCEL; HELP; }
}