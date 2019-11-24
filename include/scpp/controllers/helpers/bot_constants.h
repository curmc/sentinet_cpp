namespace scpp {
namespace curmt {

class BotConstants
{
public:
  constexpr BotConstants(const float linear_max,
                         const float linear_incriment,
                         const float angular_max,
                         const float angular_incriment)
    : linear_max(linear_max)
    , linear_incriment(linear_incriment)
    , angular_max(angular_max)
    , angular_incriment(angular_incriment)
  {}

  const float linear_max;
  const float linear_incriment;
  const float angular_max;
  const float angular_incriment;
};
}
}
