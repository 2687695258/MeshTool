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
	//填充材质创建动态数组
//	TArray<UMaterialInstanceDynamic*> DynamicMaterials_Fills;
	//保存根节点
	TSharedPtr<SBox> RootSizeBox;
	//BoxCollision复选框
	TSharedPtr<SCheckBox> CheckBox_BoxCollision;
	//光照贴图分辨率复选框
	TSharedPtr<SCheckBox> CheckBox_LightMapResolution;
	//actor材质同步mesh
	TSharedPtr<SCheckBox> CheckBox_matrial;
	//检查展开按钮
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
	//按钮光照贴图分辨率1
	TSharedPtr<SButton> Button_1;
	TSharedPtr<SHorizontalBox> hor_1;
	TSharedPtr<SVerticalBox> ver_1;
	TSharedPtr<SVerticalBox> ver_2;
	//按钮光照贴图分辨率2
	 TSharedPtr<SButton> Button_2;
	 //按钮光照贴图分辨率3
	 TSharedPtr<SButton> Button_3;
	 //按钮光照贴图分辨率4
	 TSharedPtr<SButton> Button_4;
	 //按钮光照贴图分辨率5
	 TSharedPtr<SButton> Button_5;
	 //按钮光照贴图分辨率6
	 TSharedPtr<SButton> Button_6;
	 //按钮Rename
	 TSharedPtr<SButton> B_Rename;
	 //按钮填充材质
	 TSharedPtr<SButton> B_Fill;
	 TSharedPtr<STextBlock> text_8;
	 TSharedPtr<STextBlock> text_9;
	 TSharedPtr<STextBlock> text_10;
	 TSharedPtr<STextBlock> text_11;
	 TSharedPtr<STextBlock> text_12;
	 TSharedPtr<STextBlock> text_13;
	 //光照贴图分辨率输入框
	TSharedPtr<SEditableTextBox> EditableText;
	//重命名_名称
	TSharedPtr<SEditableTextBox> Name_EditableText;
	//重命名_前缀
	TSharedPtr<SEditableTextBox> PreName_EditableText;
	//重命名_后缀
	TSharedPtr<SEditableTextBox> PostName_EditableText;
	//白名单
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
	//填充材质点击事件
	FReply OnClick_Fill();
	//Actor同步Mesh名称点击事件
	FReply OnClick_SynchronizeName();
	//检查事件
	FReply OnClick_Inspect();

	
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	bool bIsTestBoxChecked;
	bool bIsTestBoxChecked_1;
	bool CheckBox_Boxcollision;
	
	//Lod ReductionOptions 结构体
	FEditorScriptingMeshReductionOptions ReductionOptions;
	// ReductionSettings 数组 数组元素为FEditorScriptingMeshReductionSettings结构体
	TArray<FEditorScriptingMeshReductionSettings> ReductionSettings;
	FEditorScriptingMeshReductionSettings rs;
};
