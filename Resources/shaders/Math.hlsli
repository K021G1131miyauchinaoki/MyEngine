uint WangHashRand(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float3 RandVec3(uint seed_, float max_, float min_)
{
    uint rand = WangHashRand(seed_);
    float3 result;
    result.x = (rand % 1024) / 1024.0f;
    rand /= 1024;
    result.y = (rand % 1024) / 1024.0f;
    rand /= 1024;
    result.z = (rand % 1024) / 1024.0f;
    
    result.x = (max_ + abs(min_)) * result.x - abs(min_);
    result.y = (max_ + abs(min_)) * result.y - abs(min_);
    result.z = (max_ + abs(min_)) * result.z - abs(min_);
    
    if (result.x <= min_)
    {
        result.x = min_;
    }
    if (result.y <= min_)
    {
        result.y = min_;
    }
    if (result.z <= min_)
    {
        result.z = min_;
    }
    
    return result;
}