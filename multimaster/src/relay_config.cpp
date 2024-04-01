#include <multimaster/relay_config.h>

HostRelayTopicConfig::HostRelayTopicConfig(ros::NodeHandle& multimaster_nh)
  : config_pnh(multimaster_nh), RelayTopicConfig()
{
  setupTopicsList();
}

HostRelayTopicConfig::HostRelayTopicConfig() : config_pnh(), RelayTopicConfig()
{
  setupTopicsList();
}

ForeignRelayTopicConfig::ForeignRelayTopicConfig(ros::NodeHandle& multimaster_nh)
  : config_pnh(multimaster_nh), RelayTopicConfig()
{
  setupTopicsList();
}

ForeignRelayTopicConfig::ForeignRelayTopicConfig() : config_pnh(), RelayTopicConfig()
{
  setupTopicsList();
}

HostRelayTFConfig::HostRelayTFConfig(ros::NodeHandle& multimaster_nh) : config_pnh(multimaster_nh), RelayTFConfig()
{
  setupTFList();
}

HostRelayTFConfig::HostRelayTFConfig() : config_pnh(), RelayTFConfig()
{
  setupTFList();
}

ForeignRelayTFConfig::ForeignRelayTFConfig(ros::NodeHandle& multimaster_nh)
  : config_pnh(multimaster_nh), RelayTFConfig()
{
  setupTFList();
}

ForeignRelayTFConfig::ForeignRelayTFConfig() : config_pnh(), RelayTFConfig()
{
  setupTFList();
}

const vector<string>& HostRelayTopicConfig::getListTopicToSub()
{
  return topic_list_to_subscribe;
}

const vector<string>& ForeignRelayTopicConfig::getListTopicToSub()
{
  return topic_list_to_subscribe;
}

const vector<string>& HostRelayTopicConfig::getListTopicToPub()
{
  return topic_list_to_publish;
}

const vector<string>& ForeignRelayTopicConfig::getListTopicToPub()
{
  return topic_list_to_publish;
}

/**
 * Retrieves the tf list of the host tf configuration.
 *
 * @return The list of the host tf.
 */
const vector<TfTransform>& HostRelayTFConfig::getTFList()
{
  return tf_list;
}

/**
 * Retrieves the tf list of the foreign tf configuration.
 *
 * @return The list of the foreign tf.
 */
const vector<TfTransform>& ForeignRelayTFConfig::getTFList()
{
  return tf_list;
}

void HostRelayTopicConfig::addTopicPrefix(const string& prefix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    topic_list_to_publish.push_back(prefix + topic);
  }
}

void ForeignRelayTopicConfig::addTopicPrefix(const string& prefix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    topic_list_to_publish.push_back(prefix + topic);
  }
}

void HostRelayTopicConfig::stripTopicPrefix(const string& prefix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    string::size_type pos = topic.find(prefix);
    topic_list_to_publish.emplace_back(move(topic.substr(pos + prefix.size())));
  }
}

void ForeignRelayTopicConfig::stripTopicPrefix(const string& prefix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    string::size_type pos = topic.find(prefix);
    topic_list_to_publish.emplace_back(move(topic.substr(pos + prefix.size())));
  }
}

void HostRelayTopicConfig::addTopicSuffix(const string& suffix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    topic_list_to_publish.push_back(topic + suffix);
  }
}

void ForeignRelayTopicConfig::addTopicSuffix(const string& suffix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    topic_list_to_publish.push_back(topic + suffix);
  }
}

void HostRelayTopicConfig::stripTopicSuffix(const string& suffix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    string::size_type pos = topic.rfind(suffix);
    topic_list_to_publish.emplace_back(move(topic.substr(0, pos)));
  }
}

void ForeignRelayTopicConfig::stripTopicSuffix(const string& suffix)
{
  topic_list_to_publish.reserve(topic_list_to_subscribe.size());
  for (const auto& topic : topic_list_to_subscribe)
  {
    string::size_type pos = topic.rfind(suffix);
    topic_list_to_publish.emplace_back(move(topic.substr(0, pos)));
  }
}

/**
 * Set up the list of topics for subscribe at host.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void HostRelayTopicConfig::setupTopicsList()
{
  if (!config_pnh.getParam("local_pubs",
                           topic_list_to_subscribe) ||
      topic_list_to_subscribe.size() == 0)  // get list of publisher from host
  {
    ROS_ERROR("List of local publisher is not present in config or empty!");
    std::terminate();
  }

  string add_topic_prefix;
  string strip_topic_prefix;
  string add_topic_suffix;
  string strip_topic_suffix;

  if (!config_pnh.getParam("add_topic_prefix", add_topic_prefix) &&
      !config_pnh.getParam("strip_topic_prefix", strip_topic_prefix) &&
      !config_pnh.getParam("add_topic_suffix", add_topic_suffix) &&
      !config_pnh.getParam("strip_topic_suffix", strip_topic_suffix))
  {
    ROS_INFO("Nothing changed in topic names");
  }
  else
  {
    if (!add_topic_prefix.empty())
    {
      addTopicPrefix(add_topic_prefix);
    }
    if (!strip_topic_prefix.empty())
    {
      stripTopicPrefix(strip_topic_prefix);
    }
    if (!add_topic_suffix.empty())
    {
      addTopicSuffix(add_topic_suffix);
    }
    if (!strip_topic_suffix.empty())
    {
      stripTopicSuffix(strip_topic_suffix);
    }
  }

  ROS_INFO("List of local publisher is setup properly!");
}

/**
 * Sets up the list of topics for subscribe at foreign.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void ForeignRelayTopicConfig::setupTopicsList()
{
  if (!config_pnh.getParam("foreign_pubs", topic_list_to_subscribe) ||
      topic_list_to_subscribe.empty())  // get list of publisher from foreign master
  {
    ROS_ERROR("List of foreign publisher is not present in config or empty!");
    std::terminate();
  }

  string add_topic_prefix;
  string strip_topic_prefix;
  string add_topic_suffix;
  string strip_topic_suffix;

  if (!config_pnh.getParam("add_topic_prefix", add_topic_prefix) &&
      !config_pnh.getParam("strip_topic_prefix", strip_topic_prefix) &&
      !config_pnh.getParam("add_topic_suffix", add_topic_suffix) &&
      !config_pnh.getParam("strip_topic_suffix", strip_topic_suffix))
  {
    ROS_INFO("Nothing changed in topic names");
  }
  else
  {
    if (!add_topic_prefix.empty())
    {
      addTopicPrefix(add_topic_prefix);
    }
    if (!strip_topic_prefix.empty())
    {
      stripTopicPrefix(strip_topic_prefix);
    }
    if (!add_topic_suffix.empty())
    {
      addTopicSuffix(add_topic_suffix);
    }
    if (!strip_topic_suffix.empty())
    {
      stripTopicSuffix(strip_topic_suffix);
    }
  }

  ROS_DEBUG("List of foreign publisher is setup properly!");
}

/**
 * Sets up the TF list which represenatation at host.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void HostRelayTFConfig::setupTFList()
{
  vector<string> local_tf_copy;
  if (!config_pnh.getParam("local_tf", local_tf_copy) || local_tf_copy.empty())
  {
    ROS_WARN("List of local tf transformation is not present in config or empty! Ignore if you dont need it!");
    return;
  }

  tf_list.reserve(local_tf_copy.size());
  for (const auto& transform_name : local_tf_copy)
  {
    const auto delim_pos = transform_name.find("->");
    if (delim_pos != string::npos)
    {
      string src = transform_name.substr(0, delim_pos);
      string dst = transform_name.substr(delim_pos + 2);
      tf_list.emplace_back(move(TfTransform(src, dst)));
    }
  }
  ROS_INFO("List of local TF is set!");
}

/**
 * Sets up the foreign TF list which represenatation at foreign.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void ForeignRelayTFConfig::setupTFList()
{
  vector<string> foreign_tf_copy;
  if (!config_pnh.getParam("foreign_tf", foreign_tf_copy) || foreign_tf_copy.empty())
  {
    ROS_WARN("List of foreign tf transformation is not present in config or empty! Ignore if you dont need it!");
    return;
  }

  tf_list.reserve(tf_list.size() + foreign_tf_copy.size());
  for (const auto& transform_name : foreign_tf_copy)
  {
    const auto delim_pos = transform_name.find("->");
    if (delim_pos != string::npos)
    {
      string src = transform_name.substr(0, delim_pos);
      string dst = transform_name.substr(delim_pos + 2);
      tf_list.emplace_back(move(TfTransform(src, dst)));
    }
  }
  ROS_INFO("List of foreign TF is set!");
}