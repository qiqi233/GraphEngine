#pragma once
#include "EngineMinimal.h"

enum EPressState
{
	Press,//按下
	Release,//松开
};
enum class EMouseState:uint8_t
{
	RButton=0,
	LButton,
	MButton
};
struct FInputKey
{
	FInputKey()
		: Code(0)
	{
		
	}
	FInputKey(int InCode)
		: Code(InCode)
	{
		
	}
	bool operator==(const FInputKey& OtherKey)
	{
		return Code==OtherKey.Code;
	}
	bool operator!=(const FInputKey& OtherKey)
	{
		return Code != OtherKey.Code;
	}
	int Code;
	EPressState PressState;
};
namespace EKeys
{
	//F1~F12
	static FInputKey F1(VK_F1);
	static FInputKey F2(VK_F2);
	static FInputKey F3(VK_F3);
	static FInputKey F4(VK_F4);
	static FInputKey F5(VK_F5);
	static FInputKey F6(VK_F6);
	static FInputKey F7(VK_F7);
	static FInputKey F8(VK_F8);
	static FInputKey F9(VK_F9);
	static FInputKey F10(VK_F10);
	static FInputKey F11(VK_F11);
	static FInputKey F12(VK_F12);

	//A~Z
	static FInputKey A(0x41);
	static FInputKey B(0x42);
	static FInputKey C(0x43);
	static FInputKey D(0x44);
	static FInputKey E(0x45);
	static FInputKey F(0x46);
	static FInputKey G(0x47);
	static FInputKey H(0x48);
	static FInputKey I(0x49);
	static FInputKey J(0x4A);
	static FInputKey K(0x4B);
	static FInputKey L(0x4C);
	static FInputKey M(0x4D);
	static FInputKey N(0x4E);
	static FInputKey O(0x4F);
	static FInputKey P(0x50);
	static FInputKey Q(0x51);
	static FInputKey R(0x52);
	static FInputKey S(0x53);
	static FInputKey T(0x54);
	static FInputKey U(0x55);
	static FInputKey V(0x56);
	static FInputKey W(0x57);
	static FInputKey X(0x58);
	static FInputKey Y(0x59);
	static FInputKey Z(0x5A);

	//小键盘0~9
	static FInputKey NUM0(0x60);
	static FInputKey NUM1(0x61);
	static FInputKey NUM2(0x62);
	static FInputKey NUM3(0x63);
	static FInputKey NUM4(0x64);
	static FInputKey NUM5(0x65);
	static FInputKey NUM6(0x66);
	static FInputKey NUM7(0x67);
	static FInputKey NUM8(0x68);
	static FInputKey NUM9(0x69);
	
	//方向键上下左右
	static FInputKey Up(VK_UP);
	static FInputKey Down(VK_DOWN);
	static FInputKey Left(VK_LWIN);
	static FInputKey Right(VK_RIGHT);

	//其他功能键
	static FInputKey Space(VK_SPACE);
	static FInputKey Esc(VK_ESCAPE);
	static FInputKey Tab(VK_TAB);
	static FInputKey Capslock(VK_CAPITAL);

	static FInputKey Left_Shift(VK_LSHIFT);
	static FInputKey Left_Ctrl(VK_LCONTROL);
	static FInputKey Left_Alt(VK_LMENU);
	static FInputKey Left_Window(VK_LWIN);

	static FInputKey Right_Shift(VK_RSHIFT);
	static FInputKey Right_Ctrl(VK_RCONTROL);
	static FInputKey Right_Alt(VK_RMENU);
	static FInputKey Right_Window(VK_RWIN);

	//删除键
	static FInputKey DeleteKey(VK_DELETE);

	//回车键
	static FInputKey EnterKey(VK_RETURN);

	//HOME键
	static FInputKey HomeKey(VK_HOME);

	//End键
	static FInputKey EndKey(VK_END);

	//PAGE UP key
	static FInputKey PageUpKey(VK_PRIOR);
	//PAGE DOWN key
	static FInputKey PageDownKey(VK_NEXT);

	// +
	static FInputKey AddKey(VK_ADD);
	// -
	static FInputKey SubtractKey(VK_SUBTRACT);
	// *
	static FInputKey MultiplyKey(VK_MULTIPLY);
	// /
	static FInputKey DivideKey(VK_DIVIDE);

};
