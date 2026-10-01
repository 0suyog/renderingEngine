namespace Component {
class CSpeed {
public:
  CSpeed(float walkSpeed, float sprintSpeed)
      : walkSpeed(walkSpeed), sprintSpeed(sprintSpeed) {}
  float walkSpeed = 2;
  float sprintSpeed = 4;
};
} // namespace Component
