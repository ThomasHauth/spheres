#pragma once

#include <utility>
#include <iostream>
#include <cmath>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/quaternion.hpp>

typedef glm::quat Quaternion;

/**
 * Mixin for a vector with x component
 */
template<class T>
class VectorX {
public:
	/**
	 * Constructor setting the component value
	 */
	VectorX(T x) :
			m_x(x) {
	}

	/**
	 * Return the component of this vector mixin
	 */
	T x() const {
		return m_x;
	}

	/**
	 * Set the component of this vector mixin
	 */
	void setX(T v) {
		m_x = v;
	}
protected:
	/**
	 * Store the component of this vector mixin
	 */
	T m_x;
};

/**
 * Mixin for a vector with y component
 */
template<class T>
class VectorY {
public:
	/**
	 * Constructor setting the component value
	 */
	VectorY(T y) :
			m_y(y) {
	}

	/**
	 * Return the component of this vector mixin
	 */
	T y() const {
		return m_y;
	}

	/**
	 * Set the component of this vector mixin
	 */
	void setY(T v) {
		m_y = v;
	}
protected:
	/**
	 * Store the component of this vector mixin
	 */
	T m_y;
};

/**
 * Mixin for a vector with z component
 */
template<class T>
class VectorZ {
public:
	/**
	 * Constructor setting the component value
	 */
	VectorZ(T z) :
			m_z(z) {
	}

	/**
	 * Return the component of this vector mixin
	 */
	T z() const {
		return m_z;
	}

	/**
	 * Set the component of this vector mixin
	 */
	void setZ(T v) {
		m_z = v;
	}
protected:
	/**
	 * Store the component of this vector mixin
	 */
	T m_z;
};

/**
 * Base class for a n-dimensional vector of a special type
 */
template<class T>
class VectorBase {
public:
	/**
	 * Holds the base type of the vector value
	 */
	typedef T value_type;

};

/**
 * Base class for a 3d vector which stores arbitrary values
 */
template<class T>
class Vector3Base: public VectorBase<T>,
		public VectorX<T>,
		public VectorY<T>,
		public VectorZ<T> {
public:
	/**
	 * Constructor which sets all 3 components
	 */
	Vector3Base(T x, T y, T z) :
			VectorX<T>(x), VectorY<T>(y), VectorZ<T>(z) {
	}

	/**
	 * Constructor which sets all three components of the vector to zero
	 */
	Vector3Base() :
			VectorX<T>(0), VectorY<T>(0), VectorZ<T>(0) {

	}

	/**
	 * Set all components of the vector to zero
	 */
	static Vector3Base zero() {
		return Vector3Base(0, 0, 0);
	}

	/**
	 * Set the vector to the unit vector in x direction
	 */
	static Vector3Base unitX() {
		return Vector3Base(1, 0, 0);
	}

	/**
	 * Set the vector to the unit vector in y direction
	 */
	static Vector3Base unitY() {
		return Vector3Base(0, 1, 0);
	}

	/**
	 * Set the vector to the unit vector in z direction
	 */
	static Vector3Base unitZ() {
		return Vector3Base(0, 0, 1);
	}

	/**
	 * Convert to a glm vector
	 */
	glm::vec3 toGlm() const {
		return glm::vec3(this->x(), this->y(), this->z());
	}

	/**
	 * Normalize the vector
	 */
	void normalize() {
		auto l = this->length();

		this->setX(this->x() / l);
		this->setY(this->y() / l);
		this->setZ(this->z() / l);
	}

	/**
	 * Add another vector to this one and return the result
	 */
	Vector3Base & operator+=(const Vector3Base& right) {

		this->setX(this->x() + right.x());
		this->setY(this->y() + right.y());
		this->setZ(this->z() + right.z());

		return *this;
	}

	/**
	 * Multipy another vector to this one and return the result
	 */
	Vector3Base & operator*(const float factor) {

		this->setX(this->x() * factor);
		this->setY(this->y() * factor);
		this->setZ(this->z() * factor);

		return *this;
	}

	/**
	 * Return the length of the vector
	 */
	float length() const {
		return std::sqrt(
				this->x() * this->x() + this->y() * this->y()
						+ this->z() * this->z());
	}

};

/**
 * Specialization of the 3d vector to float values
 */
typedef Vector3Base<float> Vector3;

/**
 * Specialization of the 3d vector to integer values
 */
typedef Vector3Base<int> Vector3Int;

/**
 * Class which holds a 3d vector and an extra payload
 */
template<class TPayload>
class Vector3PayloadBase: public Vector3 {
public:
	/**
	 * initialize the vector with the values for x,y and z
	 */
	Vector3PayloadBase(float x, float y, float z) :
			Vector3(x, y, z) {
	}

	/**
	 * initialize the vector with the values for x,y and z
	 * and set the payload
	 */
	Vector3PayloadBase(float x, float y, float z, TPayload p) :
			Vector3(x, y, z), m_payload(p) {
	}

	/**
	 * zero initialize the vector
	 */
	Vector3PayloadBase() :
			Vector3() {

	}

	/**
	 * Set the vector payload
	 */
	void setPayload(TPayload p) {
		m_payload = p;
	}

	/**
	 * return the vector payload
	 */
	TPayload getPayload() const {
		return m_payload;
	}

private:
	/**
	 * Payload object stored in this vector in addition to
	 * x,y,z components
	 */
	TPayload m_payload;
};

/**
 * Base class for the 2d vector
 */
template<typename TData>
class Vector2Base: public VectorBase<TData>,
		public VectorX<TData>,
		public VectorY<TData> {
public:

	/**
	 * Create a 2d vector and zero initialize all components
	 */
	Vector2Base() :
			VectorX<TData>(0), VectorY<TData>(0) {

	}

	/**
	 * Create a 2d vector and set the x and y component
	 */
	Vector2Base(TData x, TData y) :
			VectorX<TData>(x), VectorY<TData>(y) {

	}
};

/**
 * 2d vector which holds unsigned integers
 */
typedef Vector2Base<size_t> Vector2UInt;

/**
 * 2d vector to hold signed integers
 */
typedef Vector2Base<int> Vector2Int;

/**
 * 2d vector which holds float values
 */
class Vector2: public Vector2Base<float> {
public:
	/**
	 * zero initialize x and y
	 */
	Vector2() :
			Vector2Base<float>(0.0f, 0.0f) {

	}

	/**
	 * Initialize the vector with x and y values
	 */
	Vector2(float x, float y) :
			Vector2Base<float>(x, y) {

	}

	/**
	 * Return a unit vector in x direction
	 */
	static Vector2 UnitX() {
		return Vector2(1.0f, 0.0f);
	}

	/**
	 * Return a unit vector in y direction
	 */
	static Vector2 UnitY() {
		return Vector2(0.0f, 1.0f);
	}

	/**
	 * Return a vector initialized with zero
	 */
	static Vector2 Zero() {
		return Vector2(0.0f, 0.0f);
	}

	/**
	 * compute and return the angle between two 2d vectors
	 */
	static float angleBetween(Vector2 const& v1, Vector2 const& v2) {
		auto mul = v1 * v2;
		auto mag = v1.mag() * v2.mag();
		return acos(mul / mag);
	}

	/**
	 * Retuns true if the vector's magnitude is larger than 0
	 * be careful, as this method only checks for a veeeery small magnitude, but not for exactly zero
	 */
	bool nonZero() const {
		return magSquared() > 0.000000001f;
	}

	/**
	 * Modify the vector so it aligns along the x axis
	 */
	void alignAlongX() {
		if (x() > 0)
			setX(mag());
		else
			setX(-mag());
		setY(0);
	}

	/**
	 * Modify the vector so it aligns along the y axis
	 */
	void alignAlongY() {
		if (y() > 0)
			setY(mag());
		else
			setY(-mag());
		setX(0);
	}

	/**
	 * Rotate the vector by 45* in clockwise direction
	 */
	Vector2 rotatePiHalfClockwiseCopy() const {
		return Vector2(y(), -x());
	}

	/**
	 * addition operator for two vectors
	 */
	Vector2 operator+(Vector2 const& other) const {
		const Vector2 newVec(this->x() + other.x(), this->y() + other.y());
		return newVec;
	}

	/**
	 * subtraction operator for two vectors
	 */
	Vector2 operator-(Vector2 const& other) const {
		const Vector2 newVec(this->x() - other.x(), this->y() - other.y());
		return newVec;
	}

	/**
	 * flip sign operator
	 */
	Vector2 operator-() const {
		const Vector2 newVec(-this->x(), -this->y());
		return newVec;
	}

	/**
	 * multiplication operator between scalar and vector
	 */
	Vector2 operator*(float factor) const {
		const Vector2 newVec(this->x() * factor, this->y() * factor);
		return newVec;
	}

	/**
	 * multiplication operator between vector and vector, will result
	 * in a scalar
	 */
	float operator*(Vector2 const& v) const {
		return this->x() * v.x() + this->y() * v.y();
	}

	/**
	 * divides the vector by a scalar
	 */
	Vector2 operator/(float div) const {
		const Vector2 newVec(this->x() / div, this->y() / div);
		return newVec;
	}

	/**
	 * Assignemnt operator
	 */
	Vector2 & operator=(Vector2 const& other) {
		this->setX(other.x());
		this->setY(other.y());
		return *this;
	}

	/**
	 * comparison operator
	 */
	bool operator==(Vector2 const& other) const {
		return (this->x() == other.x()) && (this->y() == other.y());
	}

	/**
	 * return the square of the magnitude. Saves some computation time
	 * when not doing the sqrt()
	 */
	float magSquared() const {
		return x() * x() + y() * y();
	}

	/**
	 * return the magnitude of the vector
	 */
	float mag() const {
		return sqrt(magSquared());
	}

	/**
	 * returns a normalized copy of the vector
	 */
	Vector2 normalizedCopy() const {
		const float myMag = this->mag();

		if (myMag == 0.0f) {
			return Zero();
		} else {
			const float norm = 1.0f / myMag;
			const Vector2 newVec(norm * this->x(), norm * this->y());
			return newVec;
		}
	}

	/**
	 * performs a copy of the vector and limits the maximum magnitude
	 */
	Vector2 limitedCopy(const float maxMagnitude) const {
		const float myMag = mag();
		float reductionFactor;

		if (myMag > maxMagnitude) {
			reductionFactor = maxMagnitude / myMag;
		} else {
			reductionFactor = 1.0f;
		}
		return (*this) * reductionFactor;
	}

	/**
	 * Add the x and y components of this vector to a list. Useful when preparing
	 * flat & packed data for opengl arrays
	 */
	template<class TListType>
	void addToList(TListType & l) const {
		l.push_back(x());
		l.push_back(y());
	}
};

/**
 * Convenient conversion of a vector to an output stream
 */
inline std::ostream& operator<<(std::ostream& os, Vector2 const& v) {
	os << "(" << v.x() << "|" << v.y() << ")";
	return os;
}
/**
 * first:  offset from origin
 * second: direction ( not normalized in all cases )
 */
typedef std::pair<Vector2, Vector2> Line2d;

/**
 * This class contains support code for vector usage
 */
class VectorSupport {
public:
	/**
	 * Compute the intersection between two lines.
	 * @return first item of the pair is true if an intersection was found
	 * The second entr is the intersection point.
	 */
	static std::pair<bool, Vector2> intersectLines(Line2d a, Line2d b) {
		float numerator = ((b.first.y() - a.first.y()) * a.second.x())
				+ ((a.first.x() - b.first.x()) * a.second.y());
		float denominator = (b.second.x() * a.second.y())
				- (b.second.y() * a.second.x());

		if (std::fabs(denominator) < VeeeeerySmall)
			return std::pair<bool, Vector2>(false, Vector2::UnitX());

		float t2 = numerator / denominator;

		Vector2 intersection = b.first + b.second * t2;

		return std::pair<bool, Vector2>(true, intersection);
	}

private:
	/**
	 * very small float number used for comparisons != 0
	 */
	static constexpr float VeeeeerySmall = 0.000000000001f;

};

