#pragma once
#include <queue>

namespace Hydro::win
{
	class Mouse
	{
		friend class Window;
	public:
		struct RawDelta
		{
			int x, y;
		};
		class Event
		{
		public:
			enum class Type
			{
				LPress,
				LRelease,
				RPress,
				RRelease,
				WheelUp,
				WheelDown,
				WheelPress,
				Enter,
				Leave,
				Move,
				Invalid
			};
		private:
			Type type;
			bool leftIsPressed = false;
			bool rightIsPressed = false;
			int x;
			int y;
		public:
			Event() noexcept
				:
				type( Type::Invalid ),
				leftIsPressed( false ),
				rightIsPressed( false ),
				x( 0 ),
				y( 0 )
			{}
			Event( Type type, const Mouse& parent ) noexcept
				:
				type( type ),
				leftIsPressed( parent.leftIsPressed ),
				rightIsPressed( parent.rightIsPressed ),
				x( parent.GetPosX() ),
				y( parent.GetPosY() )
			{}
			bool IsValid() const noexcept
			{
				return type != Type::Invalid;
			}
			Type GetType() const noexcept
			{
				return type;
			}
			std::pair<int, int> GetPos() const noexcept
			{
				return { x,y };
			}
			int GetPosX() const noexcept
			{
				return x;
			}
			int GetPosY() const noexcept
			{
				return y;
			}
			bool LeftIsPressed() const noexcept
			{
				return leftIsPressed;
			}
			bool RightIsPressed() const noexcept
			{
				return rightIsPressed;
			}
		};
	public:
		Mouse() = default;
		Mouse( const Mouse& ) = delete;
		Mouse& operator=( const Mouse& ) = delete;
		std::pair<int, int> GetPos() const noexcept;
		std::optional<RawDelta> ReadRawDelta() noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsInWindow() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
		Mouse::Event Read() noexcept;
		bool IsEmpty() const noexcept
		{
			return buffer.empty();
		}
		void Flush() noexcept;
		void EnableRaw() noexcept;
		void DisableRaw() noexcept;
		bool RawEnabled() const noexcept;
	private:
		void OnMouseMove( int x, int y ) noexcept;
		void OnMouseLeave() noexcept;
		void OnMouseEnter() noexcept;
		void OnRawDelta( int dx, int dy ) noexcept;
		void OnLeftPressed() noexcept;
		void OnLeftRelease() noexcept;
		void OnRightPressed() noexcept;
		void OnRightRelease() noexcept;
		void OnWheelUp() noexcept;
		void OnWheelDown() noexcept;
		void OnWheelPressed() noexcept;
		void TrimBuffer() noexcept;
		void TrimRawInputBuffer() noexcept;
		void OnWheelDelta( int delta ) noexcept;
	private:
		static constexpr unsigned int bufferSize = 16u;
		int x = 0;
		int y = 0;
		bool leftIsPressed = false;
		bool rightIsPressed = false;
		bool isInWindow = false;
		int wheelDeltaCarry = 0;
		bool rawEnabled = false;
		std::queue<Event> buffer;
		std::queue<RawDelta> rawDeltaBuffer;
	};
}