#ifndef _h_gscript_bitfield
#define _h_gscript_bitfield

template<typename T>
struct Bitfield
{
    T value;

    Bitfield(T value) : value(value) {}
    Bitfield(unsigned int value) : value(static_cast<T>(value)) {}
    operator T() const { return value; }
    operator bool() const { return static_cast<unsigned int>(value); }
    Bitfield<T> operator |(T b) { return static_cast<T>(static_cast<unsigned int>(value) | static_cast<unsigned int>(b)); };
    Bitfield<T> operator &(T b) { return static_cast<T>(static_cast<unsigned int>(value) & static_cast<unsigned int>(b)); };
    Bitfield<T> operator |=(T b) { return static_cast<T>(value = static_cast<T>(static_cast<unsigned int>(value) | static_cast<unsigned int>(b))); };
    Bitfield<T> operator &=(T b) { return static_cast<T>(value = static_cast<T>(static_cast<unsigned int>(value) & static_cast<unsigned int>(b))); };
    Bitfield<T> operator ~() { return static_cast<T>(~static_cast<unsigned int>(value)); };
};

// Enable bitwise operations on enum class
// Best to use on namespace-level
#define DEFINE_BITFIELD(Enum) \
	static inline Bitfield<Enum> operator |(Enum a, Enum b) { return static_cast<unsigned int>(a) | static_cast<unsigned int>(b); } \
	static inline Bitfield<Enum> operator &(Enum a, Enum b) { return static_cast<unsigned int>(a) & static_cast<unsigned int>(b); } \
    static inline Bitfield<Enum> operator |=(Enum& a, Enum b) { return a = static_cast<Enum>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b)); } \
	static inline Bitfield<Enum> operator &=(Enum& a, Enum b) { return a = static_cast<Enum>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b)); } \
    static inline Bitfield<Enum> operator <<(unsigned int a, Enum b) { return static_cast<Enum>(a << static_cast<unsigned int>(b)); } \
    static inline Bitfield<Enum> operator >>(unsigned int a, Enum b) { return static_cast<Enum>(a >> static_cast<unsigned int>(b)); } \
    static inline Bitfield<Enum> operator ~(Enum a) { return static_cast<Enum>(~static_cast<unsigned int>(a)); }

#endif