# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version May 28 2018)
## http://www.wxformbuilder.org/
##
## PLEASE DO *NOT* EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc

###########################################################################
## Class GateMotorInterfaceFrame
###########################################################################

class GateMotorInterfaceFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Gate Motor Interface", pos = wx.DefaultPosition, size = wx.Size( 965,564 ), style = wx.CAPTION|wx.CLOSE_BOX|wx.TAB_TRAVERSAL )
		
		self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		bSizer2 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_panel1 = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_panel1.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		bSizer3 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_panel_labelState = wx.Panel( self.m_panel1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_panel_labelState.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		self.bSizer_stateLabel = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticline1 = wx.StaticLine( self.m_panel_labelState, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		self.bSizer_stateLabel.Add( self.m_staticline1, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.m_staticText_opening = wx.StaticText( self.m_panel_labelState, wx.ID_ANY, u">> OPENING >>", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.m_staticText_opening.Wrap( -1 )
		self.m_staticText_opening.SetFont( wx.Font( 30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		self.m_staticText_opening.SetForegroundColour( wx.Colour( 0, 255, 0 ) )
		
		self.bSizer_stateLabel.Add( self.m_staticText_opening, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText_closing = wx.StaticText( self.m_panel_labelState, wx.ID_ANY, u"<< CLOSING <<", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.m_staticText_closing.Wrap( -1 )
		self.m_staticText_closing.SetFont( wx.Font( 30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		self.m_staticText_closing.SetForegroundColour( wx.Colour( 255, 0, 0 ) )
		
		self.bSizer_stateLabel.Add( self.m_staticText_closing, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticText_stopped = wx.StaticText( self.m_panel_labelState, wx.ID_ANY, u"-- STOPPED --", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.m_staticText_stopped.Wrap( -1 )
		self.m_staticText_stopped.SetFont( wx.Font( 30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		
		self.bSizer_stateLabel.Add( self.m_staticText_stopped, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticline2 = wx.StaticLine( self.m_panel_labelState, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		self.bSizer_stateLabel.Add( self.m_staticline2, 1, wx.EXPAND |wx.ALL, 5 )
		
		
		self.m_panel_labelState.SetSizer( self.bSizer_stateLabel )
		self.m_panel_labelState.Layout()
		self.bSizer_stateLabel.Fit( self.m_panel_labelState )
		bSizer3.Add( self.m_panel_labelState, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText1 = wx.StaticText( self.m_panel1, wx.ID_ANY, u"CLOSED  --|", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		self.m_staticText1.SetFont( wx.Font( 30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		
		bSizer4.Add( self.m_staticText1, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.car_image = wx.StaticBitmap( self.m_panel1, wx.ID_ANY, wx.Bitmap( u"img/car.png", wx.BITMAP_TYPE_ANY ), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.car_image, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.gate_image = wx.StaticBitmap( self.m_panel1, wx.ID_ANY, wx.Bitmap( u"img/gate.jpg", wx.BITMAP_TYPE_ANY ), wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.gate_image, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.spacer1 = wx.StaticText( self.m_panel1, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.spacer1.Wrap( -1 )
		bSizer4.Add( self.spacer1, 1, wx.ALL, 5 )
		
		self.m_staticText11 = wx.StaticText( self.m_panel1, wx.ID_ANY, u"|--  OPENED", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText11.Wrap( -1 )
		self.m_staticText11.SetFont( wx.Font( 30, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		
		bSizer4.Add( self.m_staticText11, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		bSizer3.Add( bSizer4, 0, wx.EXPAND, 5 )
		
		self.m_staticline3 = wx.StaticLine( self.m_panel1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		bSizer3.Add( self.m_staticline3, 0, wx.EXPAND |wx.ALL, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_splitter1 = wx.SplitterWindow( self.m_panel1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SP_3D )
		self.m_splitter1.Bind( wx.EVT_IDLE, self.m_splitter1OnIdle )
		
		self.m_splitter1.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		self.m_panel2 = wx.Panel( self.m_splitter1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_panel2.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer9 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_textCtrl_consoleInput = wx.TextCtrl( self.m_panel2, wx.ID_ANY, u"Send command", wx.DefaultPosition, wx.DefaultSize, wx.TE_PROCESS_ENTER )
		self.m_textCtrl_consoleInput.SetForegroundColour( wx.Colour( 0, 255, 0 ) )
		self.m_textCtrl_consoleInput.SetBackgroundColour( wx.Colour( 0, 0, 0 ) )
		
		bSizer9.Add( self.m_textCtrl_consoleInput, 1, wx.ALL, 5 )
		
		self.m_button_sendCommand = wx.Button( self.m_panel2, wx.ID_ANY, u"SEND", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer9.Add( self.m_button_sendCommand, 0, wx.ALL, 5 )
		
		
		bSizer6.Add( bSizer9, 0, wx.EXPAND, 5 )
		
		bSizer16 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_textCtrl_console = wx.TextCtrl( self.m_panel2, wx.ID_ANY, u"[ Terminal ]", wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP|wx.VSCROLL )
		self.m_textCtrl_console.SetForegroundColour( wx.Colour( 0, 255, 0 ) )
		self.m_textCtrl_console.SetBackgroundColour( wx.Colour( 0, 0, 0 ) )
		
		bSizer16.Add( self.m_textCtrl_console, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer6.Add( bSizer16, 1, wx.EXPAND, 5 )
		
		
		self.m_panel2.SetSizer( bSizer6 )
		self.m_panel2.Layout()
		bSizer6.Fit( self.m_panel2 )
		self.m_panel3 = wx.Panel( self.m_splitter1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_panel3.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		bSizer7 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer11 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_panel5 = wx.Panel( self.m_panel3, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		self.m_panel5.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		bSizer13 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer14 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_textCtrl_COM = wx.TextCtrl( self.m_panel5, wx.ID_ANY, u"COM13", wx.DefaultPosition, wx.Size( 120,-1 ), wx.TE_PROCESS_ENTER )
		bSizer14.Add( self.m_textCtrl_COM, 0, wx.ALL, 5 )
		
		self.m_button2 = wx.Button( self.m_panel5, wx.ID_ANY, u"Connect", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_button2, 0, wx.ALL, 5 )
		
		self.m_checkBox_connected = wx.CheckBox( self.m_panel5, wx.ID_ANY, u"Connected", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_checkBox_connected, 0, wx.ALL, 5 )
		
		
		bSizer13.Add( bSizer14, 0, wx.EXPAND, 5 )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText8 = wx.StaticText( self.m_panel5, wx.ID_ANY, u"Send signal", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		bSizer12.Add( self.m_staticText8, 0, wx.ALL, 5 )
		
		self.m_bpButton1 = wx.BitmapButton( self.m_panel5, wx.ID_ANY, wx.Bitmap( u"img/control.PNG", wx.BITMAP_TYPE_ANY ), wx.DefaultPosition, wx.DefaultSize, wx.BU_AUTODRAW )
		bSizer12.Add( self.m_bpButton1, 0, wx.ALL, 5 )
		
		
		bSizer13.Add( bSizer12, 0, 0, 5 )
		
		bSizer141 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_button3 = wx.Button( self.m_panel5, wx.ID_ANY, u"Request Status", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer141.Add( self.m_button3, 0, wx.ALL, 5 )
		
		self.m_checkBoxAutoRequest = wx.CheckBox( self.m_panel5, wx.ID_ANY, u"auto request", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_checkBoxAutoRequest.SetValue(True) 
		bSizer141.Add( self.m_checkBoxAutoRequest, 0, wx.ALL, 5 )
		
		
		bSizer13.Add( bSizer141, 1, wx.EXPAND, 5 )
		
		
		self.m_panel5.SetSizer( bSizer13 )
		self.m_panel5.Layout()
		bSizer13.Fit( self.m_panel5 )
		bSizer11.Add( self.m_panel5, 1, wx.EXPAND |wx.ALL, 5 )
		
		
		bSizer10.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		
		bSizer7.Add( bSizer10, 1, wx.EXPAND, 5 )
		
		
		self.m_panel3.SetSizer( bSizer7 )
		self.m_panel3.Layout()
		bSizer7.Fit( self.m_panel3 )
		self.m_splitter1.SplitVertically( self.m_panel2, self.m_panel3, 0 )
		bSizer5.Add( self.m_splitter1, 1, wx.EXPAND, 5 )
		
		
		bSizer3.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		
		self.m_panel1.SetSizer( bSizer3 )
		self.m_panel1.Layout()
		bSizer3.Fit( self.m_panel1 )
		bSizer2.Add( self.m_panel1, 1, wx.EXPAND |wx.ALL, 5 )
		
		
		self.SetSizer( bSizer2 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_KEY_DOWN, self.windowOnKeyDown )
		self.m_textCtrl_consoleInput.Bind( wx.EVT_TEXT_ENTER, self.sendCommand )
		self.m_button_sendCommand.Bind( wx.EVT_BUTTON, self.sendCommand )
		self.m_textCtrl_COM.Bind( wx.EVT_TEXT_ENTER, self.Connect_uC )
		self.m_button2.Bind( wx.EVT_BUTTON, self.Connect_uC )
		self.m_bpButton1.Bind( wx.EVT_BUTTON, self.SendSignal )
		self.m_button3.Bind( wx.EVT_BUTTON, self.RequestStatus )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def windowOnKeyDown( self, event ):
		event.Skip()
	
	def sendCommand( self, event ):
		event.Skip()
	
	
	def Connect_uC( self, event ):
		event.Skip()
	
	
	def SendSignal( self, event ):
		event.Skip()
	
	def RequestStatus( self, event ):
		event.Skip()
	
	def m_splitter1OnIdle( self, event ):
		self.m_splitter1.SetSashPosition( 0 )
		self.m_splitter1.Unbind( wx.EVT_IDLE )
	

