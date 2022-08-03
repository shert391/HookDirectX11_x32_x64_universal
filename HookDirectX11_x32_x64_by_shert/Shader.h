constexpr const char* shader = R"(
float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
    return Pos;
}

float4 PS( float4 Pos : SV_POSITION ) : SV_TARGET
{
    return float4( 26.0f, 0.0f, 255.0f, 1.0f );
}
)";
