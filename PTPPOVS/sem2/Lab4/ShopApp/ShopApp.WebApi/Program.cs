using ShopApp.Services;
using ShopApp.Core.DB;
using ShopApp.WebApi.DB;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(options =>
{
    var basePath = AppContext.BaseDirectory;
    var xmlPath = Path.Combine(basePath, "ShopApp.WebApi.xml");
    options.IncludeXmlComments(xmlPath);
});

builder.Services.AddSingleton<IProductService, ProductService>();
var connectionString = builder.Configuration.GetConnectionString("Default");
builder.Services.AddSingleton<IDataBase>(sp => new DataBase(connectionString));

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

app.UseAuthorization();

app.MapControllers();

app.Run();