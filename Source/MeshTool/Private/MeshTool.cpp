// Copyright CityColor 2024

#include "MeshTool.h"
#include "MeshToolStyle.h"
#include "MeshToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "EditorUtilityLibrary.h"
#include "EditorStaticMeshLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VRScoutingInteractor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Kismet/BlueprintPathsLibrary.h>
#include <EditorAssetLibrary.h>
#include "UObject/PropertyAccessUtil.h"
#include "Components/MeshComponent.h"
#include <Kismet/KismetStringLibrary.h>
#include <vector>

static const FName MeshToolTabName("MeshTool");

#define LOCTEXT_NAMESPACE "FMeshToolModule"

void FMeshToolModule::StartupModule()
{
	
	rs.PercentTriangles = 1.0f;
	rs.ScreenSize = 1.0f;
	ReductionOptions.ReductionSettings.Add(rs);
	rs.PercentTriangles = 0.9f;
	rs.ScreenSize = 0.5f;	
	ReductionOptions.ReductionSettings.Add(rs);
	rs.PercentTriangles = 0.8f;
	rs.ScreenSize = 0.25f;
	ReductionOptions.ReductionSettings.Add(rs);
	rs.PercentTriangles = 0.7f;
	rs.ScreenSize = 0.125f;
	ReductionOptions.ReductionSettings.Add(rs);

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMeshToolStyle::Initialize();
	FMeshToolStyle::ReloadTextures();

	FMeshToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMeshToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMeshToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMeshToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MeshToolTabName, FOnSpawnTab::CreateRaw(this, &FMeshToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMeshToolTabTitle", "MeshTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMeshToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMeshToolStyle::Shutdown();

	FMeshToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MeshToolTabName);
}

TSharedRef<SDockTab> FMeshToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMeshToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MeshTool.cpp"))
	);
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SAssignNew(RootSizeBox, SBox)
				[
					//放置overlay为了加插槽
					SNew(SOverlay)
						+ SOverlay::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Top)
						.Padding(FMargin(0, 0, 0, 0))
						[
							SNew(SScrollBox)
								+ SScrollBox::Slot()
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Top)
								.Padding(FMargin(0, 15, 0, 0))
								[
									SAssignNew(CheckBox_BoxCollision, SCheckBox)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("1", "Box Collision"))
										]
								]
								+ SScrollBox::Slot()
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Top)
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(Lod,SCheckBox)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("2", "Set Lod"))
										]
								]

								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(CheckBox_matrial,SCheckBox)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("5", "Synchronize Material"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(CheckBox_LightMapResolution, SCheckBox)
										.OnCheckStateChanged(FOnCheckStateChanged::CreateRaw(this, &FMeshToolModule::OnTestCheckboxStateChanged))
										[
											SNew(STextBlock)
												.Text(LOCTEXT("6", "Light Map Resoluion"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(CheckBox_clear,SCheckBox)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("7", "Clear Input"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(ver_1, SVerticalBox)
										.Visibility(EVisibility().Collapsed)
										+ SVerticalBox::Slot()
										[
											SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.VAlign(VAlign_Top)
												[
													SAssignNew(Button_1, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L1)
														.HAlign(HAlign_Center)
														[
															SAssignNew(text_8, STextBlock)
																.Text(LOCTEXT("8", "16"))
														]
												]
												+ SHorizontalBox::Slot()
												.VAlign(VAlign_Top)
												[
													SAssignNew(Button_2, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L2)
														.HAlign(HAlign_Center)
														[
															SAssignNew(text_9, STextBlock)
																.Text(LOCTEXT("9", "32"))
														]
												]
												+ SHorizontalBox::Slot()
												.VAlign(VAlign_Top)
												[
													SAssignNew(Button_3, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L3)
														.HAlign(HAlign_Center)
														[
															SAssignNew(text_10, STextBlock)
																.Text(LOCTEXT("10", "64"))
														]
												]
										]
										+ SVerticalBox::Slot()
										.Padding(FMargin(0, 5, 0, 0))
										[
											SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.VAlign(VAlign_Top)
												[
													SAssignNew(Button_4, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L4)
														.HAlign(HAlign_Center)
														[
															SAssignNew(text_11, STextBlock)
																.Text(LOCTEXT("11", "128"))
														]
												]
												+ SHorizontalBox::Slot()
												.VAlign(VAlign_Top)
												[
													SAssignNew(Button_5, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L5)
														.HAlign(HAlign_Center)
														[
															SAssignNew(text_12, STextBlock)
																.Text(LOCTEXT("12", "256"))
														]
												]
												+ SHorizontalBox::Slot()
												[
													SAssignNew(Button_6, SButton)
														.OnClicked_Raw(this, &FMeshToolModule::OnClick_L6)
														.HAlign(HAlign_Center)
														.VAlign(VAlign_Top)
														[
															SAssignNew(text_13, STextBlock)
																.Text(LOCTEXT("13", "512"))
														]
												]
										]
												+ SVerticalBox::Slot()
												.Padding(FMargin(0, 5, 0, 0))
												[
													SNew(SHorizontalBox)
								                + SHorizontalBox::Slot()
								               [
									               SAssignNew(EditableText,SEditableTextBox)
										           .Text(LOCTEXT("14","8"))
								               ]
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(PreName_EditableText,SEditableTextBox)
										.Text(LOCTEXT("15", "SM_"))
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 0, 0, 0))
								[
									SAssignNew(Name_EditableText,SEditableTextBox)
										.HintText(LOCTEXT("16", "Name"))
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 0, 0, 0))
								[
									SAssignNew(PostName_EditableText,SEditableTextBox)
										.Text(LOCTEXT("17", "1"))
								]

								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SNew(SButton)
										.OnClicked_Raw(this, &FMeshToolModule::RenameandSettings)
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("3", "Rename & Settings"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(B_Rename,SButton)
										.OnClicked_Raw(this, &FMeshToolModule::OnClick_Rename)
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("18", "Rename"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SNew(SButton)
										.OnClicked_Raw(this, &FMeshToolModule::OnClick_Settings)
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("19", "Settings"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SNew(SButton)
										.OnClicked_Raw(this, &FMeshToolModule::OnClick_SynchronizeName)
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("20", "Actor Synchronize Mesh Name"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))
								[
									SAssignNew(B_Fill,SButton)
										.OnClicked_Raw(this, &FMeshToolModule::OnClick_Fill)
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("21", "Filling Material"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 5, 0, 0))			
								[
									SNew(SVerticalBox)
										+SVerticalBox::Slot()
										.VAlign(VAlign_Top)
										[
											SNew(SButton)
												.OnClicked_Raw(this, &FMeshToolModule::OnClick_Inspect)
												[
													SNew(SVerticalBox)
														+ SVerticalBox::Slot()
														.HAlign(HAlign_Center)

														[
															SNew(STextBlock)
																.Text(LOCTEXT("22", "Inspect"))
														]
														+ SVerticalBox::Slot()
														.HAlign(HAlign_Left)
														[
															SNew(SCheckBox)
																.OnCheckStateChanged(FOnCheckStateChanged::CreateRaw(this, &FMeshToolModule::OnTestCheckboxStateChanged_1))
																[
																	SNew(STextBlock)
																		.Text(LOCTEXT("23", "Expand"))
																]																
														]
												]																				
										]					
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 3, 0, 0))
								[
									SAssignNew(ver_2,SVerticalBox)
										.Visibility(EVisibility().Collapsed)
									+ SVerticalBox::Slot()
									[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										[
											SAssignNew(CheckBox_name,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("24","Check Name"))
												]
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Right)
										[
											SAssignNew(CheckBox_repairname,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("25","Repair"))
												]
										]
									]
									+ SVerticalBox::Slot()
									[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										[
											SAssignNew(CheckBox_checkBox,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("26","Check Box Collision"))
												]
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Right)
										[
											SAssignNew(CheckBox_repairBox,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("27","Repair"))
												]
										]
								    ]
									+ SVerticalBox::Slot()
								    [
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										[
											SAssignNew(CheckBox_checkLod,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("28","Check LOD"))
												]
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Right)
										[
											SAssignNew(CheckBox_repairLod,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("29","Repair"))
												]
										]
								    ]
									+ SVerticalBox::Slot()
								    [
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										[
											SAssignNew(CheckBox_scale,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("30","Check Scaling"))
												]
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Right)
										[
											SAssignNew(CheckBox_repairscale,SCheckBox)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("31","Repair"))
												]
										]
								    ]
										+ SVerticalBox::Slot()
									[
										SNew(SHorizontalBox)
											+ SHorizontalBox::Slot()
											[
										SAssignNew(CheckBox_material,SCheckBox)
											[
												SNew(STextBlock)
													.Text(LOCTEXT("32","Check Empty Material"))
											]
											]
									]
										+ SVerticalBox::Slot()
									[
										SNew(SHorizontalBox)
											+ SHorizontalBox::Slot()
											[
												SAssignNew(CheckBox_mesh,SCheckBox)
												[
													SNew(STextBlock)
													.Text(LOCTEXT("33","Check Empty Mesh"))
												]
											]
									]
								+ SVerticalBox::Slot()
									[
										SNew(SHorizontalBox)
											+ SHorizontalBox::Slot()
											[
												SAssignNew(CheckBox_overriden,SCheckBox)
												[
												SNew(STextBlock)
													.Text(LOCTEXT("34","Check Overridden Light Map Resolution"))
												]
											]
									]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 3, 0, 0))
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("35", "White List"))
										]
								]
								+ SScrollBox::Slot()
								.Padding(FMargin(0, 3, 0, 0))
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										//.HAlign(HAlign_Center)
										[
											SAssignNew(WhiteList_EditableText,SEditableTextBox)
												.Text(LOCTEXT("36", ""))
										]
								]
						]
						
			]
		];
		RootSizeBox->SetWidthOverride(600);
		RootSizeBox->SetHeightOverride(833);
}
//*******************
bool FMeshToolModule::whitelist(FString name)
{
	TArray<FString> listnames;
	FString Left;
	FString Right;
	FString listname = WhiteList_EditableText->GetText().ToString() + ",";
	bool is = true;
	bool tf = true;
	while (!(listname==""))
	{
		if (is)
		{
			is = listname.Split(",", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			listnames.Add(Left);
			listname = Right;
		}
	}
	for (FString Wh : listnames)
	{
		if (name == Wh)
		{
			FText DialogText = FText::FromString(TEXT("No check"));
			EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			tf = false;
		}
	}
	return tf;
}
//*******************
FReply FMeshToolModule::RenameandSettings()
{
	FMeshToolModule::OnClick_Rename();
	FMeshToolModule::OnClick_Settings();
	return FReply::Handled();
}
//*****************************************


//Settings按钮
FReply FMeshToolModule::OnClick_Settings()
{

	//TArray<UObject*> SelectedObjects_Mesh = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<AActor*> SelectedActors = UVRScoutingInteractor::GetSelectedActors();

	//**************************************************
    //actor材质同步mesh
	bool material = CheckBox_matrial->IsChecked();
	bool Lod_check = Lod->IsChecked();
	bool lightmap_check = CheckBox_LightMapResolution->IsChecked();
	bool boxcollision = CheckBox_BoxCollision->IsChecked();

	if (material)
	{
		for (AActor* actor : SelectedActors)
		{
			
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			if (StaticMeshComponent)
			{
				UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
				for (int32 SlotIndex = 0; SlotIndex < StaticMeshComponent->GetNumMaterials(); SlotIndex++)
				{
					//获取材质
					UMaterialInterface* Material = StaticMeshComponent->GetMaterial(SlotIndex);
					UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);
					StaticMesh->SetMaterial(SlotIndex, Material); // 假设材质槽位为0//}
				}
			}
			UEditorAssetLibrary::SaveLoadedAsset(StaticMeshComponent, true);
		}
	}

	//************************************
	//	Lod
		if (Lod_check)
		{
			for (AActor* actor : SelectedActors)
			{
				UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
				if (StaticMeshComponent)
				{
					UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
					if (StaticMesh)
					{
					    UEditorStaticMeshLibrary::SetLodsWithNotification(StaticMesh, ReductionOptions, true);
					}
					UEditorAssetLibrary::SaveLoadedAsset(StaticMeshComponent, true);
				}
		}
		
	}
	    //****************************************************	


		//设置光照贴图分辨率
	FString flightmapresolution = EditableText->GetText().ToString();
	uint32 lightMapResolution = FCString::Atoi(*flightmapresolution);
	if (lightmap_check)
	{
		for (AActor* actor : SelectedActors)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			if (StaticMeshComponent)
			{
				UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
				FProperty* ObjectProp = PropertyAccessUtil::FindPropertyByName("LightMapResolution", StaticMesh->GetClass());
				UKismetSystemLibrary::Generic_SetEditorProperty(StaticMesh, ObjectProp, &lightMapResolution, ObjectProp, EPropertyAccessChangeNotifyMode::Default);
			}
			UEditorAssetLibrary::SaveLoadedAsset(StaticMeshComponent, true);
		}
			
	}

//****************************************************
    //碰撞体
	if (boxcollision)
	{
		for (AActor* actor : SelectedActors)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			if (StaticMeshComponent)
			{
				UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
				if (StaticMesh)
				{
					UEditorStaticMeshLibrary::AddSimpleCollisionsWithNotification(StaticMesh, EScriptingCollisionShapeType::Box, true);
				}
			}
			UEditorAssetLibrary::SaveLoadedAsset(StaticMeshComponent,true);
		}
	}
	return FReply::Handled();
}

//*******************************************************
//重命名--Rename按钮
FReply FMeshToolModule::OnClick_Rename()
{
		TArray<AActor*> SelectedActors_RName = UVRScoutingInteractor::GetSelectedActors();
		FString post;
		uint32 IntPost;
		post = PostName_EditableText->GetText().ToString();
		IntPost = FCString::Atoi(*post);
		if (IntPost == 0)
		{
			IntPost++;
		}
		for (AActor* actor : SelectedActors_RName)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			bool success = false;
			while (!success)
			{
				FString NewName;
				FString lastNum;
				if (IntPost < 10)
				{
					lastNum = "0" + FString::FromInt(IntPost);
				}
				else
				{
					lastNum = FString::FromInt(IntPost);
				}
				NewName = PreName_EditableText->GetText().ToString() + Name_EditableText->GetText().ToString() + "_"
					+ lastNum;
				FString pathName = UKismetSystemLibrary::GetPathName(StaticMesh);
				FString path = UBlueprintPathsLibrary::GetPath(pathName);
				FString AssetPath = path + "/" + NewName;
				success = UEditorAssetLibrary::RenameAsset(pathName, AssetPath);
				actor->SetActorLabel(NewName, true);
				IntPost++;
			}
	}
	//清空输入
	bool checkbox_clear = CheckBox_clear->IsChecked();
	if (checkbox_clear)
	{
		Name_EditableText->SetText(FText::FromString(""));
		PostName_EditableText->SetText(FText::FromString("1"));
	}
	return FReply::Handled();
}
//************************************************************

//*****************************************
// Actor同步Mesh名称
FReply FMeshToolModule::OnClick_SynchronizeName()
{
	FString synchronizeName;
	FString Left;
	FString Right;
	int32 num;
	int32 lastnum=1;
	TArray<AActor*> SelectedActors_SynchronizeName = UVRScoutingInteractor::GetSelectedActors();
	for (AActor* actor : SelectedActors_SynchronizeName)
	{
		UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		synchronizeName = StaticMesh->GetName();
		synchronizeName.Split("_", &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (UKismetStringLibrary::IsNumeric(Right))
		{
			num = FCString::Atoi(*Right);
			FString newsynchronizeName = Left + "_" + FString::FromInt(num) + "_" + FString::FromInt(lastnum);
			actor->SetActorLabel(newsynchronizeName, true);
			lastnum++;
		}
		else
		{
			int32 num1 = 1;
			FString newsynchronizeName1 = synchronizeName + FString::FromInt(num1);
			actor->SetActorLabel(newsynchronizeName1, true);
			num1++;
		}
	}
	return FReply::Handled();
}
//**********************************************************************
//填充材质
FReply FMeshToolModule::OnClick_Fill()
{
	FString MaterialName;
	FString MaterialName1;
	int Index_F = 0;
	TArray<UObject*> GetAsset_Fills = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<AActor*> SelectedActors_Fill = UVRScoutingInteractor::GetSelectedActors();
	for (AActor* actor : SelectedActors_Fill)
	{
		UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		UMeshComponent* MeshComponent = actor->FindComponentByClass<UMeshComponent>();
		//得到的材质插槽
		for (int32 m = 0, n = 0; m < MeshComponent->GetNumMaterials(); m++, n++)
		{

			MaterialName = UKismetSystemLibrary::GetDisplayName(MeshComponent->GetMaterial(m));
			if (MaterialName == "WorldGridMaterial" || MaterialName.IsEmpty())
			{
				if (n >= GetAsset_Fills.Num())
				{
					n = 0;
				}
				if (Cast<UMaterial>(GetAsset_Fills[n]))
				{
					UMaterial* material_m = Cast<UMaterial>(GetAsset_Fills[n]);
					MeshComponent->SetMaterial(m, material_m);
					StaticMesh->SetMaterial(m, material_m);
				}
				else
				{
					UMaterialInstance* material_mi = Cast<UMaterialInstance>(GetAsset_Fills[n]);
					MeshComponent->SetMaterial(m, material_mi);
					StaticMesh->SetMaterial(m, material_mi);
				}
			}
		}
	}				
	return FReply::Handled();
}
//**********************************************************************

//******************************************
//检查Lod函数
void FMeshToolModule::Isassoract(UStaticMesh* sta, bool rep)
{
	FString meshnamelod = UKismetSystemLibrary::GetDisplayName(sta);
	int32	lodcount = UEditorStaticMeshLibrary::GetLodCount(sta);
	if (lodcount <= 3)
	{
		if (rep)
		{
			UEditorStaticMeshLibrary::SetLodsWithNotification(sta, ReductionOptions, true);
		}
		else
		{
			FText DialogText = FText::FromString(meshnamelod + TEXT("did not set Lod"));
			EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		}
	}

}
FReply FMeshToolModule::OnClick_Inspect()
{
	bool checkboxcollision = CheckBox_checkBox->IsChecked();
	bool repairbox = CheckBox_repairBox->IsChecked();
	bool checklod = CheckBox_checkLod->IsChecked();
	bool repairlod = CheckBox_repairLod->IsChecked();
	bool checkoverridden = CheckBox_overriden->IsChecked();
	bool checkemptymaterial = CheckBox_material->IsChecked();
	bool checkemptymesh = CheckBox_mesh->IsChecked();
	bool checkscale = CheckBox_scale->IsChecked();
	bool repairscale = CheckBox_repairscale->IsChecked();
	bool chechname = CheckBox_name->IsChecked();
	bool repairname = CheckBox_repairname->IsChecked();


	//检查命名
	if (chechname)
	{
		FString texturename;
		TArray<UObject*> SelectObject_checkname = UEditorUtilityLibrary::GetSelectedAssets();
		TArray<FString> StringTArray;
		StringTArray.Add(FString(TEXT("StaticMesh")));
		StringTArray.Add(FString(TEXT("SkelealMesh")));
		StringTArray.Add(FString(TEXT("Material")));
		StringTArray.Add(FString(TEXT("MaterialInstanceConstant")));
		StringTArray.Add(FString(TEXT("MaterialFunction")));
		StringTArray.Add(FString(TEXT("MaterialParameterCollection")));
		StringTArray.Add(FString(TEXT("Texture2D")));
		for (UObject* obj : SelectObject_checkname)
		{
			//FMeshToolModule::whitelist(&texturename);
			UClass* class_Apa_T = obj->GetClass();
			FString className_Apa_T = UKismetSystemLibrary::GetDisplayName(class_Apa_T);
			texturename = UKismetSystemLibrary::GetDisplayName(obj);
			FString sourcepath = UKismetSystemLibrary::GetPathName(obj);
			FString path = UBlueprintPathsLibrary::GetPath(sourcepath);
			FString dirassetpath;
			bool success = false;
			FString tempName;
			uint32	index = 1;
			int32 Index_findclass = 8;
			FString numName;
			TArray<FString> CharacterArray;
			FString NewName;
			FString Apix_fix;
			const FString AllowedCharacters = "abcdefghijklmnopqrstuvwxyz1234567890_-";
			CharacterArray = UKismetStringLibrary::GetCharacterArrayFromString(texturename);
			for (size_t i = 0; i < StringTArray.Num(); i++)
			{
				if (StringTArray[i] == className_Apa_T)
				{
					Index_findclass = i;
					break;
				}
			}
			switch (Index_findclass)
			{
			case 0:
				Apix_fix = "SM_";
				break;
			case 1:
				Apix_fix = "SM_";
				break;
			case 2:
				Apix_fix = "M_";
				break;
			case 3:
				Apix_fix = "MI_";
				break;
			case 4:
				Apix_fix = "MF_";
				break;
			case 5:
				Apix_fix = "MPC_";
				break;
			case 6:
				Apix_fix = "T_";
				break;

			default:
				Apix_fix = "";
				break;
			}
			bool sta_Apa = UKismetStringLibrary::StartsWith(texturename, Apix_fix, ESearchCase::CaseSensitive);
			for (const FString ch : CharacterArray)
			{
				if (AllowedCharacters.Contains(ch))
				{
					NewName = NewName + ch;
				}
				else
				{
					if (!repairname)
					{
						FText DialogText = FText::FromString(texturename + TEXT("Naming is not standardized"));
						EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
					}
				}
			}
			if (sta_Apa)
			{
				Apix_fix = "";
			}
			else
			{
				if (!repairname)
				{
					FText DialogText = FText::FromString(texturename + TEXT("No correct prefix"));
					EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				}
			}
			NewName = Apix_fix + NewName;
			if (repairname)
			{
				dirassetpath = path + "/" + NewName + "." + NewName;
				success = UEditorAssetLibrary::RenameAsset(sourcepath, dirassetpath);
				if (!success)
				{
					while (!success)
					{
						if (index < 10)
						{
							numName = "0" + UKismetStringLibrary::Conv_IntToString(index);
						}
						else
						{
							numName = UKismetStringLibrary::Conv_IntToString(index);
						}
						if (index > 100)
						{
							break;
						}
						tempName = NewName + numName;
						dirassetpath = path + "/" + tempName + "." + tempName;
						success = UEditorAssetLibrary::RenameAsset(sourcepath, dirassetpath);
						index++;
					}
				}
			}
		}
	}

		
	
	//*****************************

	//检查碰撞体并修复
	if (checkboxcollision)
	{
		int32 convexcollision;
		int32 simplecollision;
		FString meshname;
		TArray<AActor*> SelectedActors_CheckBox = UVRScoutingInteractor::GetSelectedActors();
		for (AActor* actor : SelectedActors_CheckBox)
		{
			
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			meshname = UKismetSystemLibrary::GetDisplayName(StaticMesh);
			bool k = FMeshToolModule::whitelist(meshname);
			if (!k)
			{ 
				continue;
			}
			convexcollision = UEditorStaticMeshLibrary::GetConvexCollisionCount(StaticMesh);
			simplecollision = UEditorStaticMeshLibrary::GetSimpleCollisionCount(StaticMesh);
			if (convexcollision + simplecollision <= 0)
			{
				if (repairbox)
				{
					UEditorStaticMeshLibrary::AddSimpleCollisionsWithNotification(StaticMesh, EScriptingCollisionShapeType::Box, true);
				}
				else
				{
					FText DialogText = FText::FromString(meshname + TEXT("has no box collision"));
					EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				}
			}
		}
		TArray<UObject*> SelectedObjects_checkcollision = UEditorUtilityLibrary::GetSelectedAssets();
		for (UObject* obj : SelectedObjects_checkcollision)
		{
			UStaticMesh* StaticMesh = Cast<UStaticMesh>(obj);
			if (StaticMesh)
			{
				meshname = UKismetSystemLibrary::GetDisplayName(StaticMesh);
				bool k = FMeshToolModule::whitelist(meshname);
				if (!k)
				{
					FText DialogText = FText::FromString(meshname + TEXT(" aaaaaaaaaaaa"));
					EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
					continue;
				}
				convexcollision = UEditorStaticMeshLibrary::GetConvexCollisionCount(StaticMesh);
				simplecollision = UEditorStaticMeshLibrary::GetSimpleCollisionCount(StaticMesh);
				if (convexcollision + simplecollision <= 0)
				{
					if (repairbox)
					{
						UEditorStaticMeshLibrary::AddSimpleCollisionsWithNotification(StaticMesh, EScriptingCollisionShapeType::Box, true);
					}
					else
					{
						FText DialogText = FText::FromString(meshname + TEXT("has no box collision"));
						EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
					}
				}
			}
		}
	}

	//***************************************


	//检查Lod并修复

	if (checklod)
	{
		TArray<AActor*> SelectedActors_CheckLod = UVRScoutingInteractor::GetSelectedActors();
		for (AActor* actor : SelectedActors_CheckLod)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			if(StaticMeshComponent==nullptr)
			{
				continue;
			}
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			bool k = FMeshToolModule::whitelist(UKismetSystemLibrary::GetDisplayName(StaticMesh));
			if (!k)
			{
				continue;
			}
			FMeshToolModule::Isassoract(StaticMesh, repairlod);
		}

		TArray<UObject*> SelectedObjects_chechllod = UEditorUtilityLibrary::GetSelectedAssets();
		for (UObject* obj : SelectedObjects_chechllod)
		{
			UStaticMesh* StaticMesh1 = Cast<UStaticMesh>(obj);
			bool k = FMeshToolModule::whitelist(UKismetSystemLibrary::GetDisplayName(StaticMesh1));
			if (!k)
			{
				continue;
			}
			if (StaticMesh1)
			{
				UEditorStaticMeshLibrary::GetLodCount(StaticMesh1);
				FMeshToolModule::Isassoract(StaticMesh1, repairlod);
			}
		}
	}
//***************************************

	//检查覆盖光照贴图分辨率
	if (checkoverridden)
	{
		int32 overriddenlightmapvalue;
		FString over;
		FString overname;
		TArray<AActor*> SelectedActors_Checkoverride = UVRScoutingInteractor::GetSelectedActors();
		for (AActor* actor : SelectedActors_Checkoverride)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			bool k = FMeshToolModule::whitelist(UKismetSystemLibrary::GetDisplayName(StaticMesh));
			if (!k)
			{
				continue;
			}
			overriddenlightmapvalue = StaticMeshComponent->OverriddenLightMapRes;
			overname = UKismetSystemLibrary::GetDisplayName(actor);
			StaticMeshComponent->bOverrideLightMapRes;
			if (StaticMeshComponent->bOverrideLightMapRes)
			{
				over = FString::FromInt(overriddenlightmapvalue);
				FText DialogText = FText::FromString(overname + TEXT("'s OverriddenLightMapRes is") + over);
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		}
	}
	//************************************
	
	
	//检查空材质
	if (checkemptymaterial)
	{
		TArray<AActor*> SelectedActors_Checkemptymaterial = UVRScoutingInteractor::GetSelectedActors();
		FString materialname;
		FString emptymaterial;
		for (AActor* actor : SelectedActors_Checkemptymaterial)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			for (int32 m = 0; m < StaticMeshComponent->GetNumMaterials(); m++)
			{
				materialname = UKismetSystemLibrary::GetDisplayName(StaticMeshComponent->GetMaterial(m));
				emptymaterial = UKismetSystemLibrary::GetDisplayName(actor);
				if (materialname == "WorldGridMaterial" || materialname.IsEmpty())
				{
					FText DialogText = FText::FromString(emptymaterial + TEXT("'s") + FString::FromInt(m) + TEXT(" material slot") + TEXT("is empty"));
					EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				}
			}
		}
	}
	//**************************************
	
	//检查空网格体
	if (checkemptymesh)
	{
		FString name1;
		FString name;
		TArray<AActor*> SelectedActors_Checkemptymesh  = UVRScoutingInteractor::GetSelectedActors();
		for (AActor* actor : SelectedActors_Checkemptymesh)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			name1 = UKismetSystemLibrary::GetDisplayName(actor);
			name = UKismetSystemLibrary::GetDisplayName(StaticMesh);
			if (name.IsEmpty())
			{
				FText DialogText = FText::FromString(name1 + TEXT(" is empty "));
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		}
	}
	//***********************************


	//检查缩放
	if (checkscale)
	{
		FString Name;
		TArray<AActor*> SelectedActors_Checkscale = UVRScoutingInteractor::GetSelectedActors();
		for (AActor* actor : SelectedActors_Checkscale)
		{
			UStaticMeshComponent* StaticMeshComponent = actor->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
			bool k = FMeshToolModule::whitelist(UKismetSystemLibrary::GetDisplayName(StaticMesh));
			if (!k)
			{
				continue;
			}
			Name = UKismetSystemLibrary::GetDisplayName(actor);
			FVector Scale = actor->GetActorScale3D();
			
			if (Scale.X != 1.0f || Scale.Y != 1.0f || Scale.Z != 1.0f)
			{
				// 将缩放值修改为1		
				if (repairscale)
				{
					actor->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
				else
				{
					FText DialogText = FText::FromString(Name + TEXT(" 's scaling is not 1 "));
					EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText);
				}						
			}
		}
	}
	return FReply::Handled();
}


//隐藏光照贴图分辨率数值按钮
void FMeshToolModule::OnTestCheckboxStateChanged(ECheckBoxState NewState)
{
	bIsTestBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
	if (bIsTestBoxChecked)
	{
		ver_1->SetVisibility(EVisibility().Visible);
	}
	else
	{
		ver_1->SetVisibility(EVisibility().Collapsed);
	}
}
//隐藏检查配置
void FMeshToolModule::OnTestCheckboxStateChanged_1(ECheckBoxState NewState_1)
{
	bIsTestBoxChecked_1 = NewState_1 == ECheckBoxState::Checked ? true : false;
	if (bIsTestBoxChecked_1)
	{
		ver_2->SetVisibility(EVisibility().Visible);
	}
	else
	{
		ver_2->SetVisibility(EVisibility().Collapsed);
	}
}




FReply FMeshToolModule::OnClick_L1()
{
	EditableText->SetText(text_8->GetAccessibleText());
	
	return FReply::Handled();
}

FReply FMeshToolModule::OnClick_L2()
{
	EditableText->SetText(text_9->GetAccessibleText());
	return FReply::Handled();
}

FReply FMeshToolModule::OnClick_L3()
{
	EditableText->SetText(text_10->GetAccessibleText());
	return FReply::Handled();
}

FReply FMeshToolModule::OnClick_L4()
{
	EditableText->SetText(text_11->GetAccessibleText());
	return FReply::Handled();
}

FReply FMeshToolModule::OnClick_L5()
{
	EditableText->SetText(text_12->GetAccessibleText());
	return FReply::Handled();
}

FReply FMeshToolModule::OnClick_L6()
{
	EditableText->SetText(text_13->GetAccessibleText());
	return FReply::Handled();
}
void FMeshToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MeshToolTabName);
}

void FMeshToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMeshToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMeshToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeshToolModule, MeshTool)