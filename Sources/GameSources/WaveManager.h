#pragma once



class WaveManager
{
private:
    WaveManager() {};
    ~WaveManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();

    static WaveManager& Instance()
    {
        static WaveManager instance;
        return instance;
    }

public: // Get�֐�
    void GetNowWave();

public: // Set�֐�
    void Clear();
    void ReSet();   

private:
    
};