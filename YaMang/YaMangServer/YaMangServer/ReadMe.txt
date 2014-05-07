피드백


*보통 필터 이름은 최상위 객체의 이름을 따는것이 좋다.
예를 들어 Unit필터는 Corps가 되어야 하고 그 필터 내에는 Corps의 자식 클래스들이 존재.

같은 맥락에서 Event 필터는 Actions가 되겠쟈..

* 자꾸 만들다 보니.. ClientManager와 RoomManager사이에서 기능이 겹치거나 어디에 놔도 애매한 경우가 생기면,
그 때는 두개의 클래스를 합치거나 이름을 적절하게 바꿀 때다. 
계층 관계: RoomManager - Room or Lobby - ClientSession (Player)


* 예외 핸들링 잘 안되는 것은 아래 참고해서 장치할 것. ㅋㅋ (좋은 스터디 주제)
http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus


///# 검색 고고
