#include "pch.h"
#include "GameManager.h"
#include "CarController.h"
#include <GameObject.h>
#include <Scene.h>
#include <Builder.h>
#include <SpriteRenderer.h>
#include <ResourceManager.h>
#include <Transform.h>
#include <Texture.h>
#include <Material.h>
#include <SceneManager.h>
#include <SceneRenderHelper.h>
#include <Camera.h>

static GameObject* CreateSpriteGameObject(const wchar_t* path)
{
    Texture* tex = gResourceManager->FindAndLoad<Texture>(path);

    GameObject* obj = CreateGameObject();
    obj->AddComponent<SpriteRenderer>();
    obj->GetComponent<SpriteRenderer>()->GetMaterial(0)->SetTexture(TEX_0, tex);

    return obj;
}

GameManager::GameManager()
    : mSceneRenderHelperA(nullptr)
    , mSceneRenderHelperB(nullptr)
    , mCarA(nullptr)
    , mCarB(nullptr)
{
    constexpr int width = 800;
    constexpr int height = 210;

    mSceneRenderHelperA = new SceneRenderHelper(L"PlayerA", width, height * 2);
    mSceneRenderHelperB = new SceneRenderHelper(L"PlayerB", width, height);

    mSceneRenderHelperA->GetCamera()->GetComponent<Transform>()->SetPosition(241.f, -124.f, -2.5f);
    mSceneRenderHelperA->GetCamera()->GetComponent<Camera>()->Set2DSize(1.96f);

    //Content::initialize();
    Scene* scene = new Scene();

    {
        for (int i = 0; i < 30; ++i)
        {
            GameObject* tile = CreateSpriteGameObject(L"\\Texture\\Tho.png");
            scene->AddGameObject(tile, eLayerType::Default);
            int width = tile->GetComponent<SpriteRenderer>()->GetMaterial(0)->GetTexture(TEX_0)->GetWidth();
            tile->GetComponent<Transform>()->SetPosition(width * i, 0.f, 0.0f);
        }
        
        for (int i = 0; i < 30; ++i)
        {
            GameObject* tile = CreateSpriteGameObject(L"\\Texture\\Tho.png");
            scene->AddGameObject(tile, eLayerType::Default);
            int width = tile->GetComponent<SpriteRenderer>()->GetMaterial(0)->GetTexture(TEX_0)->GetWidth();
            tile->GetComponent<Transform>()->SetPosition(-width * i, 0.f, 0.0f);
        }

        //GameObject* tile2 = CreateSpriteGameObject(L"\\Texture\\Tho.png");
        //GameObject* tile3 = CreateSpriteGameObject(L"\\Texture\\Tho.png");
        
        //scene->AddGameObject(tile2, eLayerType::Default);
        //scene->AddGameObject(tile3, eLayerType::Default);
        
        //tile2->GetComponent<Transform>()->SetPosition(width, 0.f, 0.0f);
        //tile3->GetComponent<Transform>()->SetPosition(width * 2, 0.f, 0.0f);
    }
    {
        mCarA = CreateGameObject();
        mCarA->AddComponent<CarController>();
        mCarA->GetComponent< CarController>()->SetCamera(mSceneRenderHelperA->GetCamera());
        mCarA->GetComponent<Transform>()->SetFlipx(true);
        //OFFSET
        {                        
            GameObject* CarAA = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\SperCar\\Cars\\car_1.png");
            {
                GameObject* wheel1 = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\SperCar\\Cars\\tyre.png");
                wheel1->SetParent(CarAA);
                wheel1->GetComponent<Transform>()->SetPosition(Vector3(-314.f, -88.f, 0.f));


                GameObject* wheel2 = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\SperCar\\Cars\\tyre.png");
                wheel2->SetParent(CarAA);
                wheel2->GetComponent<Transform>()->SetPosition(Vector3(346.f, -88.f, 0.f));

                mCarA->GetComponent<CarController>()->AddWheel(wheel1);
                mCarA->GetComponent<CarController>()->AddWheel(wheel2);
            }

            CarAA->GetComponent<Transform>()->SetPosition(313.f, 0.f, 0.f);
            CarAA->SetName(L"CarAA");            
            CarAA->SetParent(mCarA);
        }        
        mCarA->SetName(L"CarA");
        mCarA->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
        mCarA->GetComponent<Transform>()->SetPosition(0.f, -250.f, 0.f);

        scene->AddGameObject(mCarA, eLayerType::Default);
    }
    
    {
        mCarB = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\Truck\\truck_1.png");
        //mCarB->AddComponent<CarController>();
        {
            GameObject* wheel1 = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\Truck\\tyre.png");            
            wheel1->SetParent(mCarB);
            wheel1->GetComponent<Transform>()->SetPosition(Vector3(-395.f, -130.f, 0.f));
            
    
            GameObject* wheel2 = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\Truck\\tyre.png");
            wheel2->SetParent(mCarB);
            wheel2->GetComponent<Transform>()->SetPosition(Vector3(190.f, -130.f, 0.f));
            
    
            GameObject* wheel3 = CreateSpriteGameObject(L"\\Texture\\asset\\Cars\\Truck\\tyre.png");
            wheel3->SetParent(mCarB);
            wheel3->GetComponent<Transform>()->SetPosition(Vector3(360.f, -130.f, 0.f));            
    
            //mCarB->GetComponent<CarController>()->AddWheel(wheel1);
            //mCarB->GetComponent<CarController>()->AddWheel(wheel2);
            //mCarB->GetComponent<CarController>()->AddWheel(wheel3);
        }
        mCarB->SetName(L"CarB");
        mCarB->GetComponent<Transform>()->SetScale(0.5f, 0.5f, 0.5f);
        mCarB->GetComponent<Transform>()->SetPosition(800.f, -420.f, 0.f);
    
        
    
        scene->AddGameObject(mCarB, eLayerType::Default);
    }


    SceneManager::GetInstance()->LoadScene(scene);
}

GameManager::~GameManager()
{
    DELETE_POINTER(mSceneRenderHelperA);
    DELETE_POINTER(mSceneRenderHelperB);
}