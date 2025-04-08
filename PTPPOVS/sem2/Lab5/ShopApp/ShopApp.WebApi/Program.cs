using ShopApp.Services;
using ShopApp.Core.DB;
using ShopApp.WebApi.DB;
using Microsoft.EntityFrameworkCore;
using ShopApp.WebApi.Infrastructure;
using ShopApp.WebApi.Services;

var builder = WebApplication.CreateBuilder(args);

// Подключение к БД
builder.Services.AddDbContext<ApplicationContext>(options =>
    options.UseSqlite(builder.Configuration.GetConnectionString("DefaultConnection")));


builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(options =>
{
    var basePath = AppContext.BaseDirectory;
    var xmlPath = Path.Combine(basePath, "ShopApp.WebApi.xml");
    options.IncludeXmlComments(xmlPath);
});

// DI для сервиса
builder.Services.AddScoped<IProductService, DataBaseProductService>();


//var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");
//builder.Services.AddSingleton<IDataBase>(sp => new DataBase(connectionString));

var app = builder.Build();

//Создаём БД при первом запуске
using (var scope = app.Services.CreateScope())
{
    var context = scope.ServiceProvider.GetRequiredService<ApplicationContext>();
    context.Database.Migrate();
}


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