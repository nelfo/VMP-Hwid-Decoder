#pragma once

#include <list>
#include <vector>
#include <string>

enum E_DecoderFlags : uint32_t
{
	DF_PROCESSORS = 1 << 0,
	DF_HOSTS = 1 << 1,
	DF_NETWORKS = 1 << 2,
	DF_DISKS = 1 << 3,
	DF_ALL = DF_PROCESSORS | DF_HOSTS | DF_NETWORKS | DF_DISKS,
};

enum E_ComponentType : uint32_t
{
	CT_PROCESSOR,
	CT_HOST,
	CT_NETWORK,
	CT_DISK,
	CT_MAX,
};

class C_Component
{
private:
	E_ComponentType m_eComponentType { CT_MAX };
	uint32_t m_u32Hash {};

public:
	C_Component( E_ComponentType eComponentType, uint32_t u32Hash )
	{
		m_eComponentType = eComponentType;
		m_u32Hash = u32Hash;
	}

	E_ComponentType& GetType( )
	{
		return m_eComponentType;
	}

	uint32_t& GetSerialHash( )
	{
		return m_u32Hash;
	}

	const char* GetName( )
	{
		switch ( GetType( ) )
		{
			case CT_PROCESSOR:
				return "Processor";
			case CT_HOST:
				return "Host";
			case CT_NETWORK:
				return "Network";
			case CT_DISK:
				return "Disk";
		}

		return "Unknown";
	}
};

namespace VMP
{
	void Base64_Encode( const unsigned char* pBuff, uint32_t u32BuffSize, std::string& cOut );
	void Base64_Decode( std::string const& cEncodedStr, std::vector< unsigned char >& cOut );

	void DecodeHwid( std::string const& cHwid, std::list < C_Component >& cOut, uint32_t u32Flags )
	{
		std::vector < unsigned char > cDecodedString {};
		Base64_Decode( cHwid, cDecodedString );

		uint32_t* pSerials = ( uint32_t* ) cDecodedString.data( );
		int iNumberOfSerials = cDecodedString.size( ) / 4;

		cOut.clear( );

		for ( int i = 0; i < iNumberOfSerials; ++i )
		{
			E_ComponentType eComponentType = E_ComponentType( pSerials[ i ] % CT_MAX );

			if ( ( 1 << eComponentType ) & u32Flags )
				cOut.push_back( C_Component( eComponentType, pSerials[ i ] - eComponentType ) );
		}

		cDecodedString.clear( );
		cDecodedString.shrink_to_fit( );
	}

	void EncodeHwid( std::list < C_Component >& cComponents, std::string& cOut )
	{
		std::vector < uint32_t > cConstructedHwid {};

		for ( auto& cComp : cComponents )
			cConstructedHwid.push_back( cComp.GetSerialHash( ) + cComp.GetType( ) );

		cOut.clear( );
		Base64_Encode( ( const unsigned char* ) cConstructedHwid.data( ), cConstructedHwid.size( ) * 4, cOut );

		cConstructedHwid.clear( );
		cConstructedHwid.shrink_to_fit( );
	}

	static const std::string cBase64Chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	void Base64_Encode( const unsigned char* pBuff, uint32_t u32BuffSize, std::string& cOut )
	{
		cOut.clear( );

		int i = 0;
		int j = 0;
		unsigned char char_array_3[ 3 ];
		unsigned char char_array_4[ 4 ];

		while ( u32BuffSize-- ) {
			char_array_3[ i++ ] = *( pBuff++ );
			if ( i == 3 ) {
				char_array_4[ 0 ] = ( char_array_3[ 0 ] & 0xfc ) >> 2;
				char_array_4[ 1 ] = ( ( char_array_3[ 0 ] & 0x03 ) << 4 ) + ( ( char_array_3[ 1 ] & 0xf0 ) >> 4 );
				char_array_4[ 2 ] = ( ( char_array_3[ 1 ] & 0x0f ) << 2 ) + ( ( char_array_3[ 2 ] & 0xc0 ) >> 6 );
				char_array_4[ 3 ] = char_array_3[ 2 ] & 0x3f;

				for ( i = 0; ( i < 4 ); i++ )
					cOut += cBase64Chars[ char_array_4[ i ] ];
				i = 0;
			}
		}

		if ( i )
		{
			for ( j = i; j < 3; j++ )
				char_array_3[ j ] = '\0';

			char_array_4[ 0 ] = ( char_array_3[ 0 ] & 0xfc ) >> 2;
			char_array_4[ 1 ] = ( ( char_array_3[ 0 ] & 0x03 ) << 4 ) + ( ( char_array_3[ 1 ] & 0xf0 ) >> 4 );
			char_array_4[ 2 ] = ( ( char_array_3[ 1 ] & 0x0f ) << 2 ) + ( ( char_array_3[ 2 ] & 0xc0 ) >> 6 );
			char_array_4[ 3 ] = char_array_3[ 2 ] & 0x3f;

			for ( j = 0; ( j < i + 1 ); j++ )
				cOut += cBase64Chars[ char_array_4[ j ] ];

			while ( ( i++ < 3 ) )
				cOut += '=';
		}
	}

	void Base64_Decode( std::string const& cEncodedStr, std::vector< unsigned char >& cOut )
	{
		auto IsBase64 = [] ( unsigned char c ) -> bool
		{
			return ( isalnum( c ) || ( c == '+' ) || ( c == '/' ) );
		};

		cOut.clear( );

		int in_len = cEncodedStr.size( );
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[ 4 ], char_array_3[ 3 ];

		while ( in_len-- && ( cEncodedStr[ in_ ] != '=' ) && IsBase64( cEncodedStr[ in_ ] ) ) {
			char_array_4[ i++ ] = cEncodedStr[ in_ ]; in_++;
			if ( i == 4 ) {
				for ( i = 0; i < 4; i++ )
					char_array_4[ i ] = cBase64Chars.find( char_array_4[ i ] );

				char_array_3[ 0 ] = ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 );
				char_array_3[ 1 ] = ( ( char_array_4[ 1 ] & 0xf ) << 4 ) + ( ( char_array_4[ 2 ] & 0x3c ) >> 2 );
				char_array_3[ 2 ] = ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ];

				for ( i = 0; ( i < 3 ); i++ )
					cOut.push_back( char_array_3[ i ] );
				i = 0;
			}
		}

		if ( i ) {
			for ( j = i; j < 4; j++ )
				char_array_4[ j ] = 0;

			for ( j = 0; j < 4; j++ )
				char_array_4[ j ] = cBase64Chars.find( char_array_4[ j ] );

			char_array_3[ 0 ] = ( char_array_4[ 0 ] << 2 ) + ( ( char_array_4[ 1 ] & 0x30 ) >> 4 );
			char_array_3[ 1 ] = ( ( char_array_4[ 1 ] & 0xf ) << 4 ) + ( ( char_array_4[ 2 ] & 0x3c ) >> 2 );
			char_array_3[ 2 ] = ( ( char_array_4[ 2 ] & 0x3 ) << 6 ) + char_array_4[ 3 ];

			for ( j = 0; ( j < i - 1 ); j++ ) cOut.push_back( char_array_3[ j ] );
		}
	}
}