import React, { useState } from "react";
import { Avatar, Card, Col, Row, Slider, Switch, Button } from "antd";
import { ApiOutlined } from "@ant-design/icons";

const { Meta } = Card;

const Labels = ({ labelsData }) => {
  return (
    <>
      <Row
        gutter={16}
        style={{ width: "90%", textAlign: "center", margin: "15px" }}
      >
        {labelsData.map((label, index) => (
          <Col span={6} key={index}>
            <Card title={label.title} bordered={false}>
              {label.content}
            </Card>
          </Col>
        ))}
      </Row>
    </>
  );
};

const Home = ({ onSliderChange, onButtonClick, onSwitch1Change, onSwitch2Change, labelsData }) => {
  const [inputValue, setInputValue] = useState(1);

  return (
    <>
      <div
        style={{
          padding: "12px",
          display: "flex",
          flexDirection: "column",
          alignItems: "center",
          justifyContent: "center", // Fix: Change justifyItems to justifyContent
        }}
      >
        <h1>CSE IoT Platform</h1>
        <Labels labelsData={labelsData} /> {/* Pass labelsData as a prop */}
        <Row
          style={{
            gap: "22px",
          }}
        >
          <Card
            style={{
              width: 300,
            }}
            actions={[
              <Switch
                checkedChildren="On"
                unCheckedChildren="Off"
                onChange={(checked) => onSwitch1Change(checked)}
              />,
            ]}
          >
            <Meta
              avatar={
                <Avatar src="https://xsgames.co/randomusers/avatar.php?g=pixel" />
              }
              title="LED1"
              description="Switch for LED1"
            />
          </Card>
          <Card
            style={{
              width: 300,
            }}
            actions={[
              <Switch
                checkedChildren="On"
                unCheckedChildren="Off"
                onChange={(checked) => onSwitch2Change(checked)}
              />,
            ]}
          >
            <Meta
              avatar={
                <Avatar src="https://xsgames.co/randomusers/avatar.php?g=pixel" />
              }
              title="LED2"
              description="Switch for LED2"
            />
          </Card>
          <Card
            style={{
              width: 300,
            }}
            actions={[
              <Slider
                min={1}
                max={20}
                onChange={(val) => {
                  setInputValue(val);
                  onSliderChange(val); // call the function from props
                }}
                value={typeof inputValue === "number" ? inputValue : 0}
              />,
            ]}
          >
            <Meta
              title="LED3"
              description="Dimming for LED3"
            />
          </Card>
          <Card
            style={{
              width: 300,
            }}
            actions={[
              <Button icon={<ApiOutlined />} onClick={onButtonClick}>
                Execute
              </Button>,
            ]}
          >
            <Meta
              title="Not yet :)"
              description="This is the description"
            />
          </Card>
        </Row>
      </div>
    </>
  );
};

export default Home;
