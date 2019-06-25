#pragma once
class Texture
{
	public:
		Texture();
		void LoadTexture(const char* c_location);
		void BindTexture();
		void UseTexture();

	private:
		unsigned int m_id;
		unsigned char* m_texture;
		int m_textureH, m_textureW, m_textureBD;
};

