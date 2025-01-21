// Copyright CityColor 2024

#pragma once

#include "EditorStaticMeshLibrary.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/SCanvas.h"
//#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

class FToolBarBuilder;
class FMenuBuilder;


class FMeshToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	//�����ʴ�����̬����
//	TArray<UMaterialInstanceDynamic*> DynamicMaterials_Fills;
	//������ڵ�
	TSharedPtr<SBox> RootSizeBox;
	//BoxCollision��ѡ��
	TSharedPtr<SCheckBox> CheckBox_BoxCollision;
	//������ͼ�ֱ��ʸ�ѡ��
	TSharedPtr<SCheckBox> CheckBox_LightMapResolution;
	//actor����ͬ��mesh
	TSharedPtr<SCheckBox> CheckBox_matrial;
	//���չ����ť
	//TSharedPtr<SCheckBox> expand;
	//LOD
	TSharedPtr<SCheckBox> Lod;
	//Clear Input
	TSharedPtr<SCheckBox> CheckBox_clear;
	//check box collision
	TSharedPtr<SCheckBox> CheckBox_checkBox;
	//repair box collision
	TSharedPtr<SCheckBox> CheckBox_repairBox;
	//check lod
	TSharedPtr<SCheckBox> CheckBox_checkLod;
	//repair lod
	TSharedPtr<SCheckBox> CheckBox_repairLod;
	//check overriden lightmap
	TSharedPtr<SCheckBox> CheckBox_overriden;
	//check empty material
	TSharedPtr<SCheckBox> CheckBox_material;
	//check empty mesh
	TSharedPtr<SCheckBox> CheckBox_mesh;
	//check scale
	TSharedPtr<SCheckBox> CheckBox_scale;
	//repair scale
	TSharedPtr<SCheckBox> CheckBox_repairscale;
	//chech name
	TSharedPtr<SCheckBox> CheckBox_name;
	//chech name
	TSharedPtr<SCheckBox> CheckBox_repairname;
	//��ť������ͼ�ֱ���1
	TSharedPtr<SButton> Button_1;
	TSharedPtr<SHorizontalBox> hor_1;
	TSharedPtr<SVerticalBox> ver_1;
	TSharedPtr<SVerticalBox> ver_2;
	//��ť������ͼ�ֱ���2
	 TSharedPtr<SButton> Button_2;
	 //��ť������ͼ�ֱ���3
	 TSharedPtr<SButton> Button_3;
	 //��ť������ͼ�ֱ���4
	 TSharedPtr<SButton> Button_4;
	 //��ť������ͼ�ֱ���5
	 TSharedPtr<SButton> Button_5;
	 //��ť������ͼ�ֱ���6
	 TSharedPtr<SButton> Button_6;
	 //��ťRename
	 TSharedPtr<SButton> B_Rename;
	 //��ť������
	 TSharedPtr<SButton> B_Fill;
	 TSharedPtr<STextBlock> text_8;
	 TSharedPtr<STextBlock> text_9;
	 TSharedPtr<STextBlock> text_10;
	 TSharedPtr<STextBlock> text_11;
	 TSharedPtr<STextBlock> text_12;
	 TSharedPtr<STextBlock> text_13;
	 //������ͼ�ֱ��������
	TSharedPtr<SEditableTextBox> EditableText;
	//������_����
	TSharedPtr<SEditableTextBox> Name_EditableText;
	//������_ǰ׺
	TSharedPtr<SEditableTextBox> PreName_EditableText;
	//������_��׺
	TSharedPtr<SEditableTextBox> PostName_EditableText;
	//������
	TSharedPtr<SEditableTextBox> WhiteList_EditableText;
    void OnTestCheckboxStateChanged(ECheckBoxState NewState);
	void OnTestCheckboxStateChanged_1(ECheckBoxState NewState_1);
	void Isassoract(UStaticMesh* sta,bool rep);
	bool whitelist(FString name);
	//check box collision

	FReply OnClick_L1();
	FReply OnClick_L2();
	FReply OnClick_L3();
	FReply OnClick_L4();
	FReply OnClick_L5();
	FReply OnClick_L6();
	FReply OnClick_Rename();
	FReply OnClick_Settings();
	FReply RenameandSettings();
	//�����ʵ���¼�
	FReply OnClick_Fill();
	//Actorͬ��Mesh���Ƶ���¼�
	FReply OnClick_SynchronizeName();
	//����¼�
	FReply OnClick_Inspect();

	
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	bool bIsTestBoxChecked;
	bool bIsTestBoxChecked_1;
	bool CheckBox_Boxcollision;
	
	//Lod ReductionOptions �ṹ��
	FEditorScriptingMeshReductionOptions ReductionOptions;
	// ReductionSettings ���� ����Ԫ��ΪFEditorScriptingMeshReductionSettings�ṹ��
	TArray<FEditorScriptingMeshReductionSettings> ReductionSettings;
	FEditorScriptingMeshReductionSettings rs;
};
