object FrmMain: TFrmMain
  Left = 9
  Top = 4
  Caption = 'WebCam using Video for Windows'
  ClientHeight = 608
  ClientWidth = 1162
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object pnlButtons: TPanel
    Left = 0
    Top = 490
    Width = 1162
    Height = 117
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      1162
      117)
    object btnSelect: TButton
      Left = 0
      Top = 8
      Width = 1162
      Height = 25
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Select &Source'
      TabOrder = 0
      OnClick = btnSelectClick
    end
    object btnDisplay: TButton
      Left = 0
      Top = 64
      Width = 1162
      Height = 25
      Anchors = [akLeft, akTop, akRight]
      Caption = '&Display Settings'
      TabOrder = 2
      OnClick = btnDisplayClick
    end
    object btnFormat: TButton
      Left = 0
      Top = 92
      Width = 1162
      Height = 25
      Anchors = [akLeft, akTop, akRight]
      Caption = '&Format Settings'
      TabOrder = 3
      OnClick = btnFormatClick
    end
    object btnSource: TButton
      Left = 0
      Top = 36
      Width = 1162
      Height = 25
      Anchors = [akLeft, akTop, akRight]
      Caption = 'S&ource Settings'
      TabOrder = 1
      OnClick = btnSourceClick
    end
  end
  object pnlCam: TPanel
    Left = 0
    Top = 0
    Width = 1162
    Height = 490
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object pnlLive: TPanel
      Left = 0
      Top = 0
      Width = 640
      Height = 480
      BevelOuter = bvNone
      TabOrder = 0
      object ImgSrc: TImage
        Left = 0
        Top = 0
        Width = 640
        Height = 480
        OnClick = imgFrameClick
      end
    end
    object pnlFrame: TPanel
      Left = 660
      Top = 0
      Width = 640
      Height = 489
      BevelOuter = bvNone
      TabOrder = 1
      object imgFrame: TImage
        Left = 0
        Top = 4
        Width = 640
        Height = 480
        OnClick = imgFrameClick
      end
    end
  end
end
