/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : pipe
 * @created     : Friday Sep 20, 2019 20:47:05 MDT
 */

#include "core/messages/pipe/PipeInterface.hpp"
#include "messages/pipe/SimpleFilter.hpp"
#include <sstream>

int
main()
{

  auto a = std::make_unique<scpp::core::PipeInterface>();

  a->create_req_rep_endpoint("2", "tcp://*:5500", "tcp://*:6500");
  a->create_req_rep_endpoint("3", "tcp://*:5501", "tcp://*:6501");
  a->create_req_rep_endpoint("4", "tcp://*:5502", "tcp://*:6502");
  a->create_req_rep_endpoint("6", "tcp://*:5503", "tcp://*:6503");
  a->create_req_rep_endpoint("7", "tcp://*:5504", "tcp://*:6504");
  a->create_req_rep_endpoint("8", "tcp://*:5505", "tcp://*:6505");
  a->create_req_rep_endpoint("9", "tcp://*:5506", "tcp://*:6506");
  a->create_req_rep_endpoint("10", "tcp://*:5507", "tcp://*:6507");
  a->create_req_rep_endpoint("11", "tcp://*:5508", "tcp://*:6508");
  a->create_req_rep_endpoint("13", "tcp://*:5509", "tcp://*:6509");
  a->create_pub_sub_endpoint("14", "tcp://*:5510", "tcp://*:6510");
  a->create_pub_sub_endpoint("16", "tcp://*:5511", "tcp://*:6511");
  a->create_pub_sub_endpoint("18", "tcp://*:5512", "tcp://*:6512");
  a->create_pub_sub_endpoint("20", "tcp://*:5513", "tcp://*:6513");
  a->create_pub_sub_endpoint("21", "tcp://*:5514", "tcp://*:6514");
  a->create_pub_sub_endpoint("22", "tcp://*:5515", "tcp://*:6515");
  a->create_pub_sub_endpoint("23", "tcp://*:5516", "tcp://*:6516");
  a->create_pub_sub_endpoint("24", "tcp://*:5517", "tcp://*:6517");
  a->create_pub_sub_endpoint("25", "tcp://*:5518", "tcp://*:6518");
  a->create_pub_sub_endpoint("27", "tcp://*:5519", "tcp://*:6519");
  a->create_pub_sub_endpoint("29", "tcp://*:5520", "tcp://*:6520");

  a->set_filter("2", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("3", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("4", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("6", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("7", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("8", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("9", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("10", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("11", std::make_unique<scpp::filters::SimpleFilter>());
  a->set_filter("13", std::make_unique<scpp::filters::SimpleFilter>());

  sleep(10);

  sleep(3);
  a->stop();

  return 0;
}
