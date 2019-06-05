# Carmelita Engine

Carmelita Engine is an Engine built in QT and C++ made by Marc López Fábregas, Adrián Castillo López and Daniel López López.

## New Features P2:
![Features](https://gyazo.com/19da2eddc8a2c3a24fd0b139b66a1ddd.PNG?raw=true)

For this delivery we've included several new features including 3D rendering using OpenGL and the hability to swap between diferent render models.

### 3D Rendering
Through establishing a path of a particular model with a set of textures we can load it directly into the scene.

Say hi to Patrick! 

![3DRendering](https://gyazo.com/44b73fd89a33dbdfef6c0b4ac7089834.PNG?raw=true)

### Lighting System
We've also added a directional Light which you can modify by selecting the option Light->Light Direction inside the Engine. You can also change the colour of that said light.

![Lighting System](https://gyazo.com/c23da3e4b44c3ec15e03479b53615d04.PNG?raw=true)

### Rendering System
There are four render modes included in this Engine: Albedo, Lighting, Normals and Depth Test.

#### Albedo:
Albedo shows the color of the object without considering any light.

![Albedo](https://gyazo.com/0c4b2646d01c740c7b3d895e41f3e323.PNG?raw=true)

### Lighting
This mode allows Defered lighting renderization of objects.

![Lighting](https://gyazo.com/d96e4eef829262e2a1ca98a014882818.PNG?raw=true)

### Normals
Display the perpendicular vector to the tangent of each polygon in the 3d models through a wide range of colours.

![Normals](https://gyazo.com/9b354f3b034b0d9a9af191374561c7f8.PNG?raw=true)

### Depth Test
Using the z-buffer of the GPU this mode shows the proximity to the near plane of the camera of each object. 
The more distinguishable a darker object is.

![DepthTest](https://gyazo.com/c3b9f8825eed1c026397f9888165883c.PNG?raw=true)

## Features P1:
![Carmelita](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/CARMELITA.PNG?raw=true)

### Hierarchy
We have a Qlist of entities and you can add and remove them with the buttons that you can see in the picture.

![Hierarchy](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Hierarchy.PNG?raw=true)

### Inspector

We have an inspector layout where you can check out and change entities' name and components.

![Inspector](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Inspecto.PNG?raw=true)

#### Component Transform

The transform component allows you to see the position, rotation and scale of an entity and modify it dinamically.

![Transform](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/Trans.PNG?raw=true)

#### Component Shape Renderer

The shape renderer component makes possible to create shapes and modify it's size, colour...

![ShapeRenderer](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/SHRENDER.PNG?raw=true)

### Save and Load

The current scene can be saved and loaded through the menu bar with the open project and save project actions. It generates a Json file with all the scene data.

![Actions](https://github.com/MarcLF/CuteTProject1/blob/master/DocumentationImages/saveload.png?raw=true)
  
## QT Features

### Signals
   
- SIGNAL(valueChanged(double)) in component transform to update it everytime a value is changed.

- SIGNAL(clicked()) in the hierarchy to add and remove entities.

- SIGNAL(itemClicked(QListWidgetItem*)) in the hierarchy to select entities.

- SIGNAL(editingFinished()) in the inspector to change the name of the entity when we finished editing it's name

- SIGNAL(triggered()) in the menu bar to perfom the actions when we click the buttons.

- SIGNAL(timeout()) in the widget to paint the scene every X time.

- SIGNAL(clicked()) in the shape renderer component to open the color picker menu.

- SIGNAL(accepted()) in the shape renderer component to appply the new color to the shape.

### Slots

#### Component Transform
- void modifyXPos(double value);
- void modifyYPos(double value);
- void modifyXRot(double value);
- void modifyYRot(double value);
- void modifyXScale(double value);
- void modifyYScale(double value);

#### Hierarchy
- void AddEntity();
- void RemoveEntity();
- void SelectEntity(QListWidgetItem* item);
- std::vector<Entity*> GetEntityList();

#### Inspector
- void NameChanged();

#### Main Window
- void openProject();
- void saveProject();

#### Paint Widget
- void paintEvent(QPaintEvent *event) override;
- void myUpdate();

#### Shape Renderer Component
- void onFillColor();
- void changeFillColor();
- void onStrokeColor();
- void changeStrokeColor();

### Style Sheet

We use the style sheet named QT Dark Fusion Style.
You can find it here : https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle

