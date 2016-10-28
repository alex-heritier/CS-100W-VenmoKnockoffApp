
#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

class Serializable {
public:
	virtual ~Serializable() {}
	virtual void inflate(const unsigned char *) = 0;
	virtual void serialize(unsigned char *) = 0;
protected:
	const int MAX_FIELD_SIZE = 64;
	unsigned char *serialize_int(unsigned char *, const unsigned int) const;
	unsigned char *serialize_char(unsigned char *, const char) const;
	unsigned char *serialize_char_array(unsigned char *, const char *, const int) const;
	int deserialize_int(const unsigned char *) const;
};

#endif
