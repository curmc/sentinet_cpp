/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ExamplePipe
 * @created     : Thursday Sep 12, 2019 20:29:56 MDT
 */

int main() {

  auto a = new ::messages::Pipe();

  // Create a new endpoint with id pub_sub1.
  // somehow, this id needs to be visible externally so that other processes can
  // interact with it
  a->add_pub_sub_endpoint("pub_sub1", "tcp://localhost:5555",
                          "tcp://localhost:5556");

  // Add filters (ideally only one per id) but the pipe would handle all the
  // conversion stuff
  a->add_filter("pub_sub1", ::messages::filters::NeuralNetworkFilter);
  a->add_filter("pub_sub1", ::messages::filters::AddOneFilter);
  a->add_filter("pub_sub1", ::messages::filters::OtherFilter);

  /*
   * Where each filter has a simple method:
   *
   * virtual std::string convert(std::string& data) = 0;
   *
   * Thus, the above would cause the pub_sub_endpoint "pub_sub1" to filter every
   * message three times
   *
   * message (string)
   * |
   * V
   * tcp://localhost:5555
   *
   * message
   * |
   * V
   * NeuralNetwork
   * |
   * V
   * AddOneFilter
   * |
   * V
   * OtherFilter
   * |
   * V
   * localhost:5556
   * |
   * V
   *
   * message (modified)
   */

  auto b = new ::messages::Pipe();
  b->add_server_endpoint("server_1", "tcp://localhost:5555",
                         "tcp://localhost:5556");

  b->start();
  b->add_filter(...); // Adding filters after wards wouldn't be as efficient,
                      // but it would insert a filter into the currently running
                      // node all during runtime

  auto c = new ::messages::Pipe();
  c->add_pub_sub_endpoint("pub_2", ...);
  c->add_filter(....);
  c->start(); // Adding filters before would prep data so that the parallel
              // ventilator can allocate space on the gpu
}
